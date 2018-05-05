//
// Created by gaston on 22/04/18.
//

#include <sys/msg.h>

#ifndef BROKER_MSG_QUEUE_H
#define BROKER_MSG_QUEUE_H

int creamsg(int id);

int getmsg(int id);

int sendmsg(int id, const void *msgp, size_t msgsz);

int rcvmsg(int id, void *msgp, size_t msgsz, long type);

int rcvmsg_no_wait(int id, void *msgp, size_t msgsz, long type);

int delmsg(int id);

#endif //BROKER_MSG_QUEUE_H
