#ifndef BROKER_MESSAGE_H
#define BROKER_MESSAGE_H

#define MAX_TOPIC_LENGTH 30
#define MAX_MSG_LENGTH 100

#define CREATE 1
#define SUBSCRIBE 2
#define PUBLISH 3
#define RECEIVE 4
#define DESTROY 5
#define ACK_OK 6
#define ACK_CREATE 7
#define ACK_DESTROY 8 //TODO when brk resp receives ACK destroy, it must remove client from broker_ids
#define ACK_ERROR 9

typedef struct payload_t {
    char topic[MAX_TOPIC_LENGTH];
    char msg[MAX_MSG_LENGTH];
} payload_t;

typedef struct msg_t {
    long mtype;

    // So that when worker responds to resp_handler, it can send the handler's pid in mtype,
    // but global_id is still in the msg. The handler couldn't lookup the global id in the table, since there could be
    // more than one global id for the same mtype (i.e. multiple clients in same machine).
    int global_id;

    int type;
    payload_t payload;
} msg_t;

#endif
