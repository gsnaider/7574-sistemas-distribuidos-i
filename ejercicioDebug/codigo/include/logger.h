#ifndef _LOGGER_H_
#define _LOGGER_H_

#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define KNRM  0
#define KRED  1
#define KGRN  2
#define KYEL  3
#define KBLU  4
#define KMAG  5
#define KCYN  6
#define KWHT  7
#define KGRY  8
#define KLRED 9
#define KLGRN 10
#define KLYEL 11
#define KLBLU 12
#define KLMAG 13
#define KLCYN 14
#define KLWHT 15

#define NUM_COLORS 16

char const *colors[] = {"\e[00m","\e[31m", "\e[32m", "\e[33m",
                        "\e[34m", "\e[35m", "\e[36m", "\e[37m",
                        "\e[90m","\e[91m", "\e[92m", "\e[93m",
                        "\e[94m", "\e[95m", "\e[96m", "\e[97m"};

static char PROCESS_NAME[100];
int _color=0;

#define FIRST_COLUMN 32

static void build_log_message(char *buffer){
    char space[50];
    memset((void*)space,*(int*)" ", 50);

    pid_t pid;
    pid=getpid();

    sprintf(buffer, "%s", colors[_color]);
    sprintf(buffer+strlen(buffer), "%s[", PROCESS_NAME);
    sprintf(buffer+strlen(buffer), "%i]:", pid);
    
    if(strlen(buffer)<FIRST_COLUMN){
        space[FIRST_COLUMN-strlen(buffer)]=0;
        sprintf(buffer+strlen(buffer), "%s", space);
    }
}

void safelog(const char* format, ...){
    char buffer[2048];

    build_log_message(buffer);

    va_list args;
    va_start(args, format);

    vsprintf(buffer+(strlen(buffer)), format, args );

    sprintf(buffer+strlen(buffer), "%s", colors[KNRM]);

    va_end(args);
    write(fileno(stdout), buffer, strlen(buffer));
}

void safeperror(const char* format, ...){
  int error = errno;
  char buffer[2048];
  build_log_message(buffer);

    va_list args;
    va_start(args, format);

    vsprintf(buffer+(strlen(buffer)), format, args );

    sprintf(buffer+strlen(buffer), ":%s %s%s\n", colors[(_color+NUM_COLORS/2)%NUM_COLORS], strerror(error), colors[KNRM]);

    va_end(args);

    write(fileno(stdout), buffer, strlen(buffer));
}

static void exit_message(void){
    safelog("!!!!! FIN DEL PROCESO !!!!!\n");
}

void log_exit(){
    atexit(exit_message);
}

void init_logger(const char* pname, int color){
    strncpy(PROCESS_NAME, pname, 99);
    _color = color%NUM_COLORS;
}


#endif /* _LOGGER_H_ */

