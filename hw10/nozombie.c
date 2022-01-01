/*
usage: ./NoZombie 10000
*/
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/wait.h>

int main(int argc, char **argv)
{
    int pid, num;
    // sscanf(argv[1], "%d", &num);
    for (int i = 0; i < 100; i++)
    {
        pid = fork();
        if (pid == 0)
        { 
            sleep(i+1);
            exit(0);
        }
        if (pid != 0)
        {
            int wret;
            wait(&wret);
        }
    }
    if (pid != 0)
        getchar();
}