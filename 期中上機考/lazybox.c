// #define st_ctime st_ctim.tv_sec
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <libgen.h>
#include <pwd.h>
#include <grp.h>
#include <fcntl.h>
char lazyBox_path[4096];

int lazyshell_main()
{

    while (1)
    {
        char command[4096];

        printf("lazybox$");
        fgets(command, 4096, stdin);
        command[strlen(command) - 1] = '\0';
        if (strcmp(command, "exit") == 0)
            exit(0);
        else if(command[0]=='c'&&command[1]=='d'){
            chdir(command+3);
            system("bash");
        }
        else
        {
            system(command);
        }
    }
}
int ln_main(char** argv,int argc)
{
    char command[1024];
    strcpy(command,"ln ");
    if(argc==4){
        // strcat(lazyBox_path,argv[2]);
        // strcpy(argv[2],lazyBox_path);
        strcat(command,"-s ");
        strcat(command,argv[2]);
        strcat(command," ");
        strcat(command,argv[3]);
    }
    else if(argc==3){
        // strcat(lazyBox_path,argv[1]);
        // strcpy(argv[1],lazyBox_path);
        strcat(command,argv[1]);
        strcat(command," ");
        strcat(command,argv[2]);
    }
    else return 0;

    system(command);

}
int stat_main(char **argv)
{
    char pathname[512];
    struct stat buf;
    int perm;
    char *c_time, *m_time, *a_time;
    strcpy(pathname, "./");
    strcat(pathname, argv[1]);
    printf("%s", pathname);
    stat(pathname, &buf);
    c_time = ctime(&buf.st_ctime);
    c_time[strlen(c_time) - 1] = 0;
    a_time = ctime(&buf.st_atime);
    a_time[strlen(a_time) - 1] = 0;
    m_time = ctime(&buf.st_mtime);
    m_time[strlen(m_time) - 1] = 0;
    printf("%8s %s %s %s\n", getpwuid(buf.st_uid)->pw_name, a_time, m_time, c_time);
}
int chown_main(char **argv, int check)
{
    char input[10];
    struct timespec file_t, current_t;
    struct stat statt;
    // stat("sudo_success", &stat);
    if (check == 0)
    {
        printf("你要執行的是特權指令,確定的話,請輸入『yes』,否則按下『enter』終止操作\n");
        fgets(input,10,stdin);
    }
    if(input[0]=='\n') return 0;
    else input[strlen(input)-1]='\0';
    if (strcmp(input, "yes") == 0 || check == 1)
    {
        char command[1024];
        strcpy(command, "chown");
        strcat(command, " ");
        strcat(command, argv[1]);
        strcat(command, " ");
        strcat(command, argv[2]);
        setuid(0);
        system(command);
    }
    else
    {
        return 0;
    }
}
int main(int argc, char **argv)
{
    getcwd(lazyBox_path, 4096);
    argv[0] = basename(argv[0]);
    int chownnum = 0;
    if (strcmp(argv[0], "lazyshell") == 0)
    {
        lazyshell_main();
    }
    else if(strcmp(argv[0],"ln")==0){

        ln_main(argv,argc);
    }
    else if (strcmp(argv[0], "stat") == 0)
    {
        stat_main(argv);
    }
    else if (strcmp(argv[0], "chown") == 0)
    {
        struct timespec file_t, current_t;
        struct stat statt;
        char path[1024];
        strcpy(path,"./");
        strcat(path,argv[2]);
        stat(path, &statt);
        file_t = statt.st_ctim;
        clock_gettime(CLOCK_REALTIME, &current_t);
        long num=current_t.tv_sec-file_t.tv_sec;
        if(num<=30) chownnum=1;
        strcat(lazyBox_path,argv[2]);
        strcpy(argv[2],lazyBox_path);
        // printf("%s\n",lazyBox_path);
        chownnum = chown_main(argv, chownnum);
    }
}