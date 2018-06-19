//
// Created by gaston on 25/05/18.
//

#include <stdbool.h>
#include <unistd.h>
#include <wait.h>
#include <stdlib.h>
#include "../common/ipc/sig.h"
#include "../common/log/log.h"
#include "../common/ds/data-structures/list.h"
#include "../common/ipc/socket.h"
#include "db_server.h"
#include "../common/ipc/semaphore.h"


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

void safe_exit(int socket, int sem, int error) {
    log_info("Stopping server.");

    kill_childs();

    log_debug("Destroying childs pids list.");
    list_destroy(&childs);

    log_debug("Closing socket.");
    if(close(socket) < 0) {
        log_error("Error closing db socket.");
        error = -1;
    }

    log_debug("Closing DB sem.");
    if (delsem(sem) < 0) {
        log_error("Error deleting DB sem.");
        error = -1;
    }

    exit(error);
}

int main(int argc, char* argv[]) {
    register_handler(SIGINT_handler);

    log_info("Starting Database.");

    int sem = creasem(DB_SEM);
    if (sem < 0) {
        log_error("Error creating DB sem.");
        exit(-1);
    }
    inisem(sem, 1);

    list_new(&childs, sizeof(pid_t), NULL);

    int socket = create_server_socket(DB_PORT);
    if (socket < 0) {
        safe_exit(socket, sem, -1);
    }

    while (!graceful_quit) {
        log_info("Waiting connections to DB.");
        int client_socket = accept_client(socket);
        if (graceful_quit || client_socket < 0) {
            break;
        }
        log_info("Client connected.");
        pid_t handler = fork();
        if (handler < 0) {
            log_error("Error forking DB handler.");
            safe_exit(socket, sem, -1);
        }
        if (handler == 0) {
            if (close(socket) < 0) {
                log_error("Error closing DB socket.");
            }
            char client_socket_str[3];
            snprintf(client_socket_str, 3, "%d", client_socket);
            execl("./db_handler", "./db_handler", client_socket_str, (char*)NULL);
            log_error("Error executing DB handler.");
            safe_exit(socket, sem, -1);
        }
        list_append(&childs, &handler);
        if (close(client_socket) < 0) {
            log_error("Error closing client socket.");
        }
    }

    safe_exit(socket, sem, 0);


}