#include<stdio.h>
#include<string.h>
#include<libgen.h>
#include<stdlib.h>
#include<unistd.h>
int main(int argc,char** argv){
    char* exename = basename(argv[0]);
    printf("%s",exename);
    system(exename);
}
