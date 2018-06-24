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
#include "global_id.h"
#include "ring_connection.h"

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

void create_global_ids() {
    if (global_ids_create() < 0) {
        log_error("Error creating global ids table.");
        exit(-1);
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

    int ring_connection_setup_queue = creamsg(RING_CONNECTION_SETUP_QUEUE);
    log_debug("Ring queue %d created", ring_connection_setup_queue);
    if (ring_connection_setup_queue < 0) {
        log_error("Error creating ring connection setup queue.");
        exit(-1);
    }
}

void destroy_global_ids() {
    int global_ids = global_ids_get();
    if (global_ids < 0) {
        log_error("Error getting global ids for deletion.");
    } else {
        global_ids_destroy(global_ids);
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

    int ring_connection_setup_queue = getmsg(RING_CONNECTION_SETUP_QUEUE);
    if (ring_connection_setup_queue < 0) {
        log_error("Error getting ring_connection_setup_queue for deletion.");
    } else {
        delmsg(ring_connection_setup_queue);
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

void safe_exit(int socket, int error) {
    log_info("Stopping server.");

    kill_childs();

    log_debug("Destroying childs pids list.");
    list_destroy(&childs);

    log_debug("Destroying queues");
    destroy_queues();

    log_debug("Destroying global ids");
    destroy_global_ids();

    log_debug("Closing socket.");
    if(close(socket) < 0) {
        log_error("Error closing server socket.");
    }

    exit(error);
}

void fork_req_handler(int server_socket, int client_socket) {
    pid_t handler = fork();
    if (handler < 0) {
        log_error("Error forking handler.");
        safe_exit(server_socket, -1);
    }
    if (handler == 0) {
        if (close(server_socket) < 0) {
            log_error("Error closing server socket.");
        }
        char client_socket_str[3];
        snprintf(client_socket_str, 3, "%d", client_socket);
        execl("./req_handler", "./req_handler", client_socket_str, (char*)NULL);
        log_error("Error executing request handler.");
        safe_exit(server_socket, -1);
    }
    list_append(&childs, &handler);
    if (close(client_socket) < 0) {
        log_error("Error closing client socket.");
    }
}

int wait_ring_connection(int socket) {
    log_info("Waiting ring connection.");
    int previous_server_socket = accept_client(socket);
    if (previous_server_socket < 0) {
        log_error("Error receiving ring connection.");
        safe_exit(socket, -1);
    }
    log_info("Ring connection received.");
    return previous_server_socket;

}

int fork_ring_connection_proc() {
    pid_t ring_connection = fork();
    if (ring_connection < 0) {
        log_error("Error forking handler.");
        return -1;
    }
    if (ring_connection == 0) {
        execl("./ring_connection", "./ring_connection", (char*)NULL);
        log_error("Error executing ring connection process.");
        return -1;
    }
    return ring_connection;
}

int main(int argc, char* argv[]) {

    register_handler(SIGINT_handler);

    log_info("Server started.");

    list_new(&childs, sizeof(pid_t), NULL);

    create_global_ids();
    create_queues();
    create_workers();

    int socket = create_server_socket(PORT);
    if (socket < 0) {
        safe_exit(socket, -1);
    }
    int ring_connection_pid = fork_ring_connection_proc();
    if (ring_connection_pid < 0){
        log_error("Error forking ring connection process.");
        safe_exit(socket, -1);
    }

    // First connection will always be from ring server.
    int previous_server_socket = wait_ring_connection(socket);

    // For now, an extra resp handler will be created within this req_handler, but it will never be called.
    // This req_handler will only receive redirected PUBLISH msgs form other servers, and never a CREATE message.
    // Thus, the resp handler will never be added to the global ids table.
    // TODO (optional) see if this extra resp handler can be avoided. (maybe creating both req and resp within the server).
    fork_req_handler(socket, previous_server_socket);

    waitpid(ring_connection_pid, (int*) NULL, 0);

    int ring_connection_setup_queue = getmsg(RING_CONNECTION_SETUP_QUEUE);
    log_debug("Ring queue %d got.", ring_connection_setup_queue);

    ring_setup_msg_t msg;
    rcvmsg(ring_connection_setup_queue, &msg, sizeof(ring_setup_msg_t), 0);

    log_info("Resp handler '%d' created by ring connection.", msg.resp_handler_pid);
    list_append(&childs, &msg.resp_handler_pid);


    while (!graceful_quit) {
        log_info("Waiting connections.");
        int client_socket = accept_client(socket);
        if (graceful_quit || client_socket < 0) {
            break;
        }
        log_info("Client connected.");
        fork_req_handler(socket, client_socket);
    }

    safe_exit(socket, 0);

}
