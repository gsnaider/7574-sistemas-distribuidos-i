//
// Created by gaston on 19/06/18.
//

#ifndef BROKER_RING_CONNECTION_H
#define BROKER_RING_CONNECTION_H

#define NEXT_SERVER_IP "127.0.0.1"
#define NEXT_SERVER_PORT 8080

#define NEXT_SERVER_GLOBAL_ID 0


typedef struct ring_setup_msg_t {
    long mtype;
    pid_t resp_handler_pid;
} ring_setup_msg_t;


#endif //BROKER_RING_CONNECTION_H