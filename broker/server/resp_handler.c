//
// Created by gaston on 22/04/18.
//

#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include "../common/log/log.h"
#include "../common/ipc/sig.h"
#include "../common/ipc/msg_queue.h"
#include "server.h"
#include "../common/message.h"
#include "../common/ipc/socket.h"

bool graceful_quit = false;

void SIGINT_handler(int signum) {
    if (signum != SIGINT) {
        log_warn("WARNING: Unknown signal received: %d.", signum);
    } else {
        log_debug("SIGINT received, aborting.");
        graceful_quit = true;
    }
}

void process_msg(int socket, msg_t *msg) {
    if (msg->type != ACK_OK && msg->type != ACK_ERROR && msg->type != ACK_CREATE && msg->type != PUBLISH) {
        log_error("Invalid msg type (%d) received.", msg->type);
        return;
    }
    log_info("Message received of type %d.", msg->type);
    // Set the global_id (previously set by worker) on the msg mtype.
    msg->mtype = msg->global_id;
    socket_send(socket, msg);

}

int main(int argc, char* argv[]) {
    register_handler(SIGINT_handler);
    log_info("Starting response handler.");
    if (argc < 2) {
        log_error("No client socket parameter received.");
        exit(-1);
    }
    int client_socket;
    sscanf(argv[1], "%d", &client_socket);

    int resp_queue = getmsg(RESP_QUEUE);

    while(!graceful_quit) {
        log_info("Waiting response messages from worker...");
        msg_t msg;
        rcvmsg(resp_queue, &msg, sizeof(msg_t), getpid());
        if (graceful_quit) {
            break;
        }
        process_msg(client_socket, &msg);
    }

    //TODO(optional) kill req handler if we get his pid.
    log_info("Stopping response handler.");
    if (close(client_socket) < 0) {
        log_error("Error closing client socket.");
    }
}