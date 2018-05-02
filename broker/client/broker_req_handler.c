//
// Created by gaston on 22/04/18.
//

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <memory.h>
#include "../common/log/log.h"
#include "../common/ipc/msg_queue.h"
#include "broker_handler.h"
#include "../common/ipc/socket.h"
#include "../common/ipc/shm.h"
#include "../common/message.h"
#include "../common/ipc/semaphore.h"

void create_broker_resp_handler(int socket_fd);

int init_msg_req_queue();

int init_msg_resp_queue();

msg_t* create_msg_shm() {
    int msg_shm = creashm(INCOMING_MSG_SHM, sizeof(msg_t) * MAX_INCOMING_MSG);
    if (msg_shm < 0) {
        log_error("Error getting msg shm.");
        exit(-1);
    }
    return map(msg_shm);
}

int* create_msg_count_shm() {
    int msg_count_shm = creashm(INCOMING_MSG_COUNT_SHM, sizeof(int));
    if (msg_count_shm < 0) {
        log_error("Error getting msg count shm.");
        exit(-1);
    }
    return map(msg_count_shm);
}

int create_incoming_msg_sem() {
    int incoming_msgs_sem = creasem(INCOMING_MSG_SEM);
    if (incoming_msgs_sem < 0) {
        log_error("Error getting msg sem.");
        exit(-1);
    }
    inisem(incoming_msgs_sem, 1);
    return incoming_msgs_sem;
}

int main(int argc, char* argv[]) {
    log_info("Starting local broker Request Handler.");

    int socket_fd = create_client_socket(SERVER_IP, SERVER_PORT);
    log_info("Connected to server");

    msg_t* incoming_msgs = create_msg_shm();
    int* incoming_msg_count = create_msg_count_shm();
    int incoming_msgs_sem = create_incoming_msg_sem();
    int req_queue = init_msg_req_queue();
    int resp_queue = init_msg_resp_queue();

    create_broker_resp_handler(socket_fd);

    // TODO while(true) read from req_queue and send msg to server.

    sleep(10);

    log_info("Msg type %d", incoming_msgs[0].type);
    log_info("Msg type %d", incoming_msgs[MAX_INCOMING_MSG + 1].type);



    log_debug("Deleting message queue.");
    delmsg(req_queue);
    delmsg(resp_queue);

    log_debug("Deleting incoming messages shared memory.");
    unmap(incoming_msgs);
    delshm(getshm(INCOMING_MSG_SHM));
    unmap(incoming_msg_count);
    delshm(getshm(INCOMING_MSG_COUNT_SHM));

    log_debug("Deleting msg shm semaphore");
    delsem(incoming_msgs_sem);

    log_debug("Closing socket.");
    close(socket_fd);
}

int init_msg_req_queue() {
    if (creamsg(BROKER_REQ_MSG) < 0) {
        log_error("Error creating req msg queue.");
        exit(-1);
    }
    int req_queue = getmsg(BROKER_REQ_MSG);
    if (req_queue < 0) {
        log_error("Error getting req message queue.");
        exit(-1)    ;
    }
    return req_queue;
}


int init_msg_resp_queue() {

    if (creamsg(BROKER_RESP_MSG) < 0) {
        log_error("Error creating resp msg queue.");
        exit(-1);
    }
    int resp_queue = getmsg(BROKER_RESP_MSG);
    if (resp_queue < 0) {
        log_error("Error getting resp message queue");
        exit(-1);
    }
    return resp_queue;

}

void create_broker_resp_handler(int socket_fd) {

    pid_t pid = fork();
    if (pid < 0) {
        log_error("Error forking broker resp handler");
        perror("Error");
        exit(-1);
    }
    if (pid == 0) {
        char socket_str[3];
        snprintf(socket_str, 3, "%d", socket_fd);
        execl("./broker_resp_handler", "./broker_resp_handler", socket_str, (char*)NULL);
        log_error("Error executing broker resp handler");
        perror("Error");
        exit(-1);
    }

}