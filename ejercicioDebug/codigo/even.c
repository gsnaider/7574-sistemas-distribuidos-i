#include <stdio.h>
#include <unistd.h>

#include "include/shm.h"
#include "include/msg.h"
#include "include/semaforo.h"
#include "include/logger.h"

struct Mensaje {
    long mtype;
    char data[100];

    Mensaje(){
        memset((void*)data,0,sizeof(data));
    }
};

int main(int argc, char** argv){
    int orden, n;

    int shm = getshm(0);
    int sem = getsem(0);
    int wait_sem = getsem(1);
    int msg = getmsg(0);

    int* memoria = (int*)map(shm);

    sscanf(argv[1],"%d", &orden);
    sscanf(argv[2],"%d", &n);

    int total=0;
    p(sem);{
        total = memoria[1];
    }v(sem);

    if(orden == total/2){
        Mensaje m;
        m.mtype=total;
        enviarmsg(msg, &m, sizeof(m));
    }

    Mensaje m;
    recibirmsg(msg,&m,sizeof(m),total-orden);
    char buf[5];
    printf("%c", n+m.data[orden]);

	if(m.mtype>1){
        m.mtype--;
        enviarmsg(msg, &m, sizeof(m));
    }

    p(sem);{
        if(++memoria[0] == total)
            v(wait_sem);
    }v(sem);

    return 0;
}
