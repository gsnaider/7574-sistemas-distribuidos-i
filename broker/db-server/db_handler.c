//
// Created by gaston on 25/05/18.
//


#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include "../common/ipc/sig.h"
#include "../common/log/log.h"
#include "db_message.h"
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

void safe_exit(int client_socket, int error) {
    log_debug("Closing socket");
    if (close(client_socket) < 0) {
        log_error("Error closing client socket.");
    }
    exit(error);
}

void process_message(db_msg_t *msg) {
    log_info("Received type: %d", msg->type);
    //TODO check message type and call db.operation.
}

int main(int argc, char* argv[]) {
    register_handler(SIGINT_handler);
    log_info("Starting DB handler.");

    if (argc < 2) {
        log_error("No client socket parameter received.");
        exit(-1);
    }
    int client_socket;
    sscanf(argv[1], "%d", &client_socket);

    db_msg_t msg;
    log_info("Waiting for messages from client...");
    int res = socket_receive_db(client_socket, &msg);
    if(graceful_quit) {
        safe_exit(client_socket, -1);
    }
    if (res < 0) {
        log_error("Error reading client socket.");
        safe_exit(client_socket, -1);
    } else if (res == 0) {
        log_error("Lost connection from client.");
        safe_exit(client_socket, -1);
    }

    process_message(&msg);
}