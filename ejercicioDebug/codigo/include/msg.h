#ifndef _MSG_H_
#define _MSG_H_

#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#include "logger.h"
#include "resources.h"

int creamsg(int id){
    key_t clave;
    clave = ftok(DIRECTORY, id);
    return (msgget(clave,  IPC_CREAT | IPC_EXCL | 0660));
    /* da error si ya existe */
}

int getmsg(int id){
    key_t clave;
    clave = ftok(DIRECTORY, id);
    return (msgget(clave, 0660));
}

void enviarmsg(int id, const void *msgp, size_t msgsz){
    if(msgsnd(id,msgp,msgsz-sizeof(long),0)==-1){
        safeperror("No se puede enviar el mensaje");
        exit(-1);
    }
}

void recibirmsg(int id, void *msgp, size_t msgsz, long type){
    if(msgrcv(id,msgp,msgsz-sizeof(long),type,0)==-1){
        safeperror("No se puede recibir el mensaje");
        exit(-1);
    }
}

int   elimsg(int id){
    return (msgctl(id, IPC_RMID, NULL));
}

#endif /* _MSG_H_ */
