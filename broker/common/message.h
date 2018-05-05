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
#define ACK_ERROR 8

typedef struct payload_t {
    char topic[MAX_TOPIC_LENGTH];
    char msg[MAX_MSG_LENGTH];
} payload_t;

typedef struct msg_t {
    long mtype;
    int type;
    payload_t payload;
} msg_t;

#endif
