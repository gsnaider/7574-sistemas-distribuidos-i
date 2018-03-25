#ifndef _MSG_H_
#define _MSG_H_

#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <errno.h>

#include "logger.h"
#include "resources.h"

int creamsg(int id){
    key_t clave;
    clave = ftok(DIRECTORY, id);
    if (clave < 0) {
        printf("Error on ftok while creating message queue: %s\n", strerror(errno));
        return -1;
    }
    /* da error si ya existe */
    int msg_id = msgget(clave,  IPC_CREAT | IPC_EXCL | 0660);
    if (msg_id < 0) {
        printf("Error on msgget while creating message queue: %s\n", strerror(errno));
    }
    return msg_id;
}

int getmsg(int id){
    key_t clave;
    clave = ftok(DIRECTORY, id);
    if (clave < 0) {
        printf("Error on ftok while getting message queue: %s\n", strerror(errno));
        return -1;
    }
    int msg_id = msgget(clave, 0660);
    if (msg_id < 0) {
        printf("Error on msgget while getting message queue: %s\n", strerror(errno));
    }
    return msg_id;
}

void sendmsg(int id, const void *msgp, size_t msgsz){
    if(msgsnd(id,msgp,msgsz-sizeof(long),0)==-1){
        printf("Error sending message over queue: %s\n", strerror(errno));
    }
}

void rcvmsg(int id, void *msgp, size_t msgsz, long type){
    if(msgrcv(id,msgp,msgsz-sizeof(long),type,0)==-1){
        printf("Error receiving message over queue: %s\n", strerror(errno));
    }
}

int   delmsg(int id){
    int res = msgctl(id, IPC_RMID, NULL);
    if (res < 0) {
        printf("Error deleting message queue: %s\n", strerror(errno));
    }
    return res;
}

#endif /* _MSG_H_ */
