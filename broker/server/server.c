//
// Created by gaston on 22/04/18.
//

#include <unistd.h>
#include <stdbool.h>
#include <stdlib.h>
#include <wait.h>
#include "../common/ipc/socket.h"
#include "server.h"
#include "../common/log/log.h"
#include "../common/ipc/sig.h"
#include "../common/ipc/msg_queue.h"
#include "../common/ds/data-structures/list.h"

bool graceful_quit = false;

list childs;

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

void destroy_queues() {
    int worker_queue = getmsg(WORKER_QUEUE);
    if (worker_queue < 0) {
        log_error("Error getting worker queue for deletion.");
    } else {
        delmsg(worker_queue);
    }

    int resp_queue = getmsg(RESP_QUEUE);
    if (resp_queue < 0) {
        log_error("Error getting resp queue for deletion.");
    } else {
        delmsg(resp_queue);
    }
}

void create_workers() {
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
        list_append(&childs, &worker);
    }
}

bool kill_child(void* child) {
    pid_t child_pid = *((pid_t*) child);
    log_debug("Killing %d", child_pid);
    if (kill(child_pid, SIGINT) < 0) {
        log_error("Error killing child %d.", child_pid);
    } else {
        waitpid(child_pid, (int*) NULL, 0);
    }
    return true;
}

void kill_childs() {
    list_for_each(&childs, kill_child);
}

int main(int argc, char* argv[]) {

    register_handler(SIGINT_handler);

    log_info("Server started.");

    list_new(&childs, sizeof(pid_t), NULL);

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
        list_append(&childs, &handler);
        if (close(client_socket) < 0) {
            log_error("Error closing client socket.");
        }
    }

    log_info("Stopping server.");

    kill_childs();

    log_debug("Destroying childs pids list.");
    list_destroy(&childs);

    log_debug("Destroying queues");
    destroy_queues();

    log_debug("Closing socket.");
    if(close(socket) < 0) {
        log_error("Error closing server socket.");
    }
}
