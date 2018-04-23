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
#include "../common/message.h"

void create_broker_resp_handler();

int init_msg_queue();

int main(int argc, char* argv[]) {
    log_info("Starting local broker Request Handler.");

    // TODO open socket and send to resp process.

    int socket_fd = create_client_socket(SERVER_IP, SERVER_PORT);
    log_info("Connected to server");

    msg_t msg;
    msg.type = CREATE;
    log_info("Sending create req to server.");
    if (write(socket_fd, &msg, sizeof(msg)) < 0) {
        log_error("Error writing to socket.");
    }

    // TODO dont close here!
    close(socket_fd);

    // TODO send socket to resp handler
    // TODO create shm for incoming msgs and send to resp process.
    create_broker_resp_handler();

    int req_queue = init_msg_queue();

    log_debug("Deleting message queue");
    delmsg(req_queue);
}

int init_msg_queue() {
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

void create_broker_resp_handler() {

    pid_t pid = fork();
    if (pid < 0) {
        log_error("Error forking broker resp handler");
        perror("Error");
        exit(-1);
    }
    if (pid == 0) {
        execl("./broker_resp_handler", "./broker_resp_handler", (char*)NULL);
        log_error("Error executing broker resp handler");
        perror("Error");
        exit(-1);
    }

}