#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
int main(int argc,char** argv){
    struct timespec start, end;
    FILE* input =fopen(argv[1],"r");
    FILE* output=fopen(argv[2],"w+");
    char* wordbuf; 
    int buflen=0,bufsize;
    sscanf(argv[3], "%d", &bufsize);
    if(bufsize==0){
        setvbuf(input, NULL, _IONBF, 0);
        wordbuf=malloc(80);
        setvbuf(output, NULL, _IONBF, 0);
    }
    else if(bufsize==-1){
        setvbuf(input, wordbuf, _IOLBF, 80);
        wordbuf=malloc(80);
        setvbuf(output, wordbuf, _IOLBF, 80);
    }
    else {
        setvbuf(input, NULL, _IOFBF,bufsize);
        wordbuf=malloc(bufsize);
        setvbuf(output, wordbuf, _IOFBF,bufsize);
    }
    int linepos=0,blank=0;
    clock_gettime(CLOCK_MONOTONIC, &start);
    while(1){
        int inputInt=getc(input);
        if(inputInt==EOF || inputInt=='\0') break;
        if(inputInt>127) continue;
        unsigned char inputChar=(unsigned char) inputInt;
        if(inputChar=='\n'){
            wordbuf[buflen]='\0';
            fprintf(output,"%s",wordbuf);
            fprintf(output,"\n");
            linepos=0;
            buflen=0;
            continue;
        }
        if(inputChar==' '){
            if(buflen==0) continue;
            blank=1;
            if(linepos+buflen>=80){
                fprintf(output,"\n");
                linepos=0;
            }
            wordbuf[buflen]='\0';
            fprintf(output,"%s",wordbuf);
            linepos=linepos+buflen;
            buflen=0;
            continue;
        }
        if(blank==1&&linepos!=0) wordbuf[buflen++]=' ';
        if(buflen+linepos>=80){
            wordbuf[0]='\n';
            linepos=0;
        }
        wordbuf[buflen++]=inputChar;
        blank=0;
    }
    clock_gettime(CLOCK_MONOTONIC, &end);
    printf("%fs\n",end.tv_sec-start.tv_sec+(double)(end.tv_nsec-start.tv_nsec)/1000000000.0);
    wordbuf[buflen]='\0';
    fprintf(output,"%s",wordbuf);
}