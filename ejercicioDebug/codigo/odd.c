#include <stdio.h>
#include <unistd.h>
#include "include/logger.h"

#include "include/msg.h"
#include "include/shm.h"
#include "include/semaforo.h"

struct Mensaje {
    long mtype;
    char data[100];

    Mensaje(){
        memset((void*)data,0,sizeof(data));
    }
};


int main(int argc, char** argv){

    srand(getpid());
	char buffer[3];

    int orden, n;

    int msg = getmsg(0);
    int shm = getshm(0);
    int sem = getsem(0);

    int* memoria = (int*)map(shm);

    sscanf(argv[1],"%d", &orden);
    sscanf(argv[2],"%d", &n);

    int total=0;
    p(sem);{
        total = memoria[1];
    }v(sem);

    Mensaje m;
    recibirmsg(msg,&m,sizeof(m),total-orden);
    sprintf(buffer, "%s", colors[(m.mtype+rand())%14+1]);
    printf("%s", buffer);

    enviarmsg(msg,&m,sizeof(m));
	execv("./procesar_2n", argv);
    recibirmsg(msg,&m,sizeof(m),0);
    return 0;
}
