#define _GNU_SOURCE
#include<sched.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <signal.h>
long long int cpp = 0;
void alarmHandler(int signo)
{
    printf("cpp = %lld\n", cpp);
    exit(0);
}
int main(int argc, char **argv)
{
    // int nice_v = atoi(argv[1]); //讀入 nice 的參數
    cpu_set_t mask; //CPU 核的集合
    CPU_ZERO(&mask);
    CPU_SET(1, &mask); //先做好參數設定,綁在第一顆處理器
    sched_setaffinity(0, sizeof(mask), &mask);
    int childPid = fork(); //產生二個行程
    if (childPid > 0)
    {
        //養我們的雙親
        nice(5); //修改 parent 的 nice 值
    }
    else
    {
        //child 不用特別處理
    }
    //底下的程式碼無論 child 是否大於 0 都會執行
    //設定鬧鐘(SIGALRM)叫的時候, 『作業系統』呼叫alarmHandler
    signal(SIGALRM, alarmHandler);
    //把鬧鐘的時間定在 1 秒鐘以後
    alarm(1);
    //不斷地 cpp++
    while (1)
    {
        cpp++;
    }
}
