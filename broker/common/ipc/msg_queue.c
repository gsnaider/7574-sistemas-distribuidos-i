//
// Created by gaston on 22/04/18.
//

#include "msg_queue.h"
#include "resources.h"
#include "../log/log.h"

int creamsg(int id){
    key_t clave = ftok(DIRECTORY, id);
    if (clave < 0) {
        log_error("Error on ftok while creating message queue");
        return -1;
    }
    /* da error si ya existe */
    int msg_id = msgget(clave,  IPC_CREAT | IPC_EXCL | 0660);
    if (msg_id < 0) {
        log_error("Error on msgget while creating message queue");
    }
    return msg_id;
}

int getmsg(int id){
    key_t clave;
    clave = ftok(DIRECTORY, id);
    if (clave < 0) {
        log_error("Error on ftok while getting message queue");
        return -1;
    }
    int msg_id = msgget(clave, 0660);
    if (msg_id < 0) {
        log_error("Error on msgget while getting message queue");
    }
    return msg_id;
}

int sendmsg(int id, const void *msgp, size_t msgsz){
    if(msgsnd(id,msgp,msgsz-sizeof(long),0)==-1){
        log_error("Error sending message over queue");
        return -1;
    }
    return 0;
}

int rcvmsg(int id, void *msgp, size_t msgsz, long type){
    if(msgrcv(id,msgp,msgsz-sizeof(long),type,0)==-1){
        log_error("Error receiving message over queue");
        return -1;
    }
    return 0;
}

int   delmsg(int id){
    int res = msgctl(id, IPC_RMID, NULL);
    if (res < 0) {
        log_error("Error deleting message queue");
    }
    return res;
}

