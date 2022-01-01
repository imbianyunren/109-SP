#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<sys/types.h>
#include<sys/file.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<libgen.h>
#include <unistd.h>
#include <assert.h>
#define BUF_SIZE 4096
int main(int argc,char** argv){
    int fd;
    char buffer[BUF_SIZE];
    int offset=0;
    fd=open("./lockf.db",O_RDWR,S_IRUSR|S_IWUSR);
    for(int i=0;i<=1000;i++){
        lockf(fd,F_LOCK,4);
        lseek(fd,0,SEEK_END);
        lseek(fd,-4,SEEK_CUR);
        read(fd,buffer,4);
        sscanf(buffer,"%d",&offset);
        lseek(fd,0,SEEK_END);
        lseek(fd,offset,SEEK_CUR);
        sprintf(buffer,"%d",offset+1);
        write(fd,buffer,4);
        lockf(fd,F_UNLCK,4);
        usleep(10000);
    }
}