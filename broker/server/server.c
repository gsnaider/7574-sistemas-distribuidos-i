//
// Created by gaston on 22/04/18.
//

#include <unistd.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../common/ipc/socket.h"
#include "server.h"
#include "../common/log/log.h"
#include "../common/ipc/sig.h"
#include "../common/ipc/msg_queue.h"

bool graceful_quit = false;

void SIGINT_handler(int signum) {
    if (signum != SIGINT) {
        log_warn("WARNING: Unknown signal received: %d.", signum);
    } else {
        log_debug("SIGINT received, aborting.");
        graceful_quit = true;
    }
}

void create_queues() {
    int worker_queue = creamsg(WORKER_QUEUE);
    if (worker_queue < 0) {
        log_error("Error creating worker queue.");
        exit(-1);
    }

    int resp_queue = creamsg(RESP_QUEUE);
    if (resp_queue < 0) {
        log_error("Error creating response queue.");
        exit(-1);
    }
}

void create_workers() {
    // TODO store child pids (workers and handlers) in array for killing them in case of terminating signal.
    for (int i = 0; i < WORKERS; i++) {
        pid_t worker = fork();
        if (worker < 0) {
            log_error("Error creating worker.");
            exit(-1);
        }
        if (worker == 0) {
            execl("./worker", "./worker", (char*)NULL);
            log_error("Error executing worker.");
            exit(-1);
        }
    }
}

int main(int argc, char* argv[]) {

    register_handler(SIGINT_handler);

    log_info("Server started.");

    create_queues();
    create_workers();

    int socket = create_server_socket(PORT);

    while (!graceful_quit) {
        log_info("Waiting connections.");
        int client_socket = accept_client(socket);
        if (graceful_quit || client_socket < 0) {
            break;
        }
        log_info("Client connected.");
        pid_t handler = fork();
        if (handler < 0) {
            log_error("Error forking handler.");
            exit(-1);
        }
        if (handler == 0) {
            if (close(socket) < 0) {
                log_error("Error closing server socket.");
            }
            char client_socket_str[3];
            snprintf(client_socket_str, 3, "%d", client_socket);
            execl("./req_handler", "./req_handler", client_socket_str, (char*)NULL);
            log_error("Error executing request handler.");
            exit(-1);
        }
        if (close(client_socket) < 0) {
            log_error("Error closing client socket.");
        }
    }

    log_info("Stopping server.");
    if(close(socket) < 0) {
        log_error("Error closing server socket.");
    }
}
