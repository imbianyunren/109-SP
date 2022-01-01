#include <sys/inotify.h>
#include <stdio.h>
#include <dirent.h>
#include <unistd.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/types.h>
#include <time.h>
#include <stdio.h>

//設定每次read最多讀取1000個物件，這裡如果設定太小，可能會有「漏失」某些事件
#define BUF_LEN (1000 * (sizeof(struct inotify_event) + NAME_MAX + 1))

//key-value的映射，陣列的形式，key就是第一個index
//最多可以映射1000個，value最多是10000個字母
char wd[1000][4000];

int parseInotifyEvent(struct inotify_event *event)
{
	// int eatRet;
	// char buf[8192] = "";
	// printf("@event = %p\n", event);
	// sprintf(buf, "來自[%s]的事件 ", wd[event->wd]);
	// printf("%s\n", wd[event->wd]);
	//底下是將所有的事件做檢查，照理說應該只會有一個事件
	// strncat(buf + strlen(buf), "{", 4095);
	if (event->mask & IN_ACCESS)
		return 0;
	if (event->mask & IN_ATTRIB)
		return 0;
	//應該只有底下二個事件改變了檔案內容
	if (event->mask & IN_CLOSE_WRITE)
		return 1;
	if (event->mask & IN_MODIFY)
		return 1;
	return 0;
}

void getMTime(int fd, struct tm *tm)
{
	struct stat statbuf;
	struct tm t;
	char buf[1000];
	fstat(fd, &statbuf);
	tzset();
	localtime_r(&(statbuf.st_mtime), tm);
}

int main(int argc, char **argv)
{
	//監聽的頻道
	int fd, readFd;
	int nRead = 1, ret, i;
	char *eventPtr;
	char *inotify_entity = (char *)malloc(BUF_LEN);
	struct tm last_mtime, cur_mtime;
	char curtimeStr[100], lasttimeStr[100];
	char c;
	//跟作業系統要一個監聽專用的『頻道』，作業系統會幫我們建立一個檔案，
	//用這個檔案「送」資料給我們，並且自動開啟該「檔案/頻道」，並給它的fd
	fd = inotify_init();
	//設定在哪些檔案監聽哪些事件
	ret = inotify_add_watch(fd, argv[1], IN_ALL_EVENTS);
	if (ret != -1)
		strcpy(wd[ret], argv[1]);
	readFd = open(argv[1], O_RDONLY);
	getMTime(readFd, &cur_mtime);
	getMTime(readFd, &last_mtime);
	strftime(curtimeStr, 1000, "%F %T", &cur_mtime);
	strftime(lasttimeStr, 1000, "%F %T", &cur_mtime);
	printf("\n這次修改時間：%s, 上次修改時間：%s\n", curtimeStr, lasttimeStr);
	FILE *fp = fdopen(readFd, "r");
	assert(fp != NULL);
	char readBuf[1000];
	assert(fgets(readBuf, 1000, fp) != NULL);
	char str[100];
	strcpy(str,"cat ");
	strcat(str,argv[1]);
	strcat(str,"\0");
	//使用一個while loop不斷地讀取 inotify_init() 所開啟的檔案 fd
	//fd 裏面就是我們要監聽的訊息
	while (1)
	{
		int isModify = 0;
		//一直讀取，作業系統開給我們的頻道，nRead是這次頻道中的資料量大小
		nRead = read(fd, inotify_entity, BUF_LEN);

		// printf("從與作業系統的秘密檔案通道讀到『%d』個字元\n", nRead);
		//底下的 for loop 不斷地將收進來的資料切割成『不定長度的』的 inotify_event
		// printf("這些字元的解析如下\n");
		for (eventPtr = inotify_entity; eventPtr < inotify_entity + nRead;)
		{

			isModify = parseInotifyEvent((struct inotify_event *)eventPtr);
			//😜 🤪 🤨 🧐當isModify為1時應該要印出時間和資料😜 🤪 🤨 🧐
			strftime(lasttimeStr, 1000, "%F %T", &last_mtime);
			getMTime(readFd, &cur_mtime);
			strftime(curtimeStr, 1000, "%F %T", &cur_mtime);
			if (isModify == 1)
			{
				printf("\n這次修改時間：%s, 上次修改時間：%s\n", curtimeStr, lasttimeStr);
				last_mtime = cur_mtime;
				system(str);
			}
			/*
            struct inotify_event {
                int      wd;       // Watch descriptor 
                uint32_t mask;     // Mask describing event 
                uint32_t cookie;   // Unique cookie associating related
                                   //     events (for rename(2))
                uint32_t len;      // Size of name field 🦐 🦞 🦀 在這裏OS會告訴我們name的長度
                char     name[];   // Optional null-terminated name
                };
            */
			//目前這個物件的長度是 基本的inotiry_event的長度 ＋ name字串的長度
			//將eventPtr加上物件長度，就是下一個物件的開始位置
			eventPtr += sizeof(struct inotify_event) + ((struct inotify_event *)eventPtr)->len;
		}
	}
}