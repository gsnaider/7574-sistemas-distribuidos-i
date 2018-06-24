//
// Created by gaston on 19/06/18.
//

#include <stdbool.h>
#include <unistd.h>
#include <stdlib.h>
#include "ring_connection.h"
#include "../common/ipc/sig.h"
#include "../common/log/log.h"
#include "server.h"
#include "../common/ipc/socket.h"
#include "global_id.h"
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

pid_t create_resp_handler(int socket) {
    pid_t pid = fork();
    if (pid < 0) {
        log_error("Error forking resp handler");
        exit(-1);
    }
    if (pid == 0) {
        char socket_str[3];
        snprintf(socket_str, 3, "%d", socket);
        execl("./resp_handler", "./resp_handler", socket_str, (char*)NULL);
        log_error("Error executing resp handler");
        exit(-1);
    }
    if (close(socket) < 0) {
        log_error("Error closing next server socket.");
    }
    return pid;
}

int main(int argc, char* argv[]) {
    register_handler(SIGINT_handler);

    log_info("Starting ring connection.");

    int next_server_socket = create_client_socket(NEXT_SERVER_IP, NEXT_SERVER_PORT);
    log_info("Ring connection established to next server.");

    pid_t resp_handler_pid = create_resp_handler(next_server_socket);

    int global_ids = global_ids_get();
    if (add_global_id(global_ids, NEXT_SERVER_GLOBAL_ID, resp_handler_pid) < 0) {
        log_error("Error adding next ring server to global ids.");
        exit(-1);
    }

    int ring_connection_setup_queue = getmsg(RING_CONNECTION_SETUP_QUEUE);
    ring_setup_msg_t msg;
    msg.resp_handler_pid = resp_handler_pid;
    sendmsg(ring_connection_setup_queue, &msg, sizeof(ring_setup_msg_t));
    return 0;
}