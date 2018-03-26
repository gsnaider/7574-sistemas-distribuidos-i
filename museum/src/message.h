#ifndef _MESSAGE_H_
#define _MESSAGE_H_

#define ENTRANCE_REQUEST 1
#define EXIT_REQUEST 2
#define ACCEPT 3
#define REJECT 4
#define TOUR_REQUEST 5
#define TOUR_START 6

typedef struct message_t {
    long mtype;
    int type;
} message_t;

#endif