//
// Created by gaston on 22/04/18.
//

#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <wait.h>
#include "../common/log/log.h"
#include "../common/message.h"
#include "../common/ipc/sig.h"
#include "../common/ipc/socket.h"
#include "../common/ipc/msg_queue.h"
#include "server.h"


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
    return pid;
}

int get_worker_queue() {
    int worker_queue = getmsg(WORKER_QUEUE);
    if (worker_queue < 0) {
        log_error("Error getting worker queue.");
        exit(-1);
    }
    return worker_queue;
}

void process_message(int queue, msg_t *msg, int resp_handler_pid) {
    if (msg->type == ACK_OK || msg->type == ACK_ERROR || msg->type == ACK_CREATE || msg->type == ACK_DESTROY || msg->type == ACK_NO_MSG || msg->type == RECEIVE) {
        log_error("Unexpected msg type received: %d", msg->type);
        return;
    }
    if (msg->type == CREATE) {
        msg->mtype = resp_handler_pid;
    }
    log_info("Sending message to worker.");
    if (sendmsg(queue, msg, sizeof(msg_t)) < 0) {
        log_error("Error sending message to worker.");
    }
}

int main(int argc, char* argv[]) {
    register_handler(SIGINT_handler);
    log_info("Starting request handler.");
    if (argc < 2) {
        log_error("No client socket parameter received.");
        exit(-1);
    }
    int client_socket;
    sscanf(argv[1], "%d", &client_socket);

    pid_t resp_handler_pid = create_resp_handler(client_socket);

    int worker_queue = get_worker_queue();

    while(!graceful_quit) {
        msg_t msg;
        log_info("Waiting for messages from client...");
        int res = socket_receive(client_socket, &msg);
        if(graceful_quit) {
            break;
        }
        if (res < 0) {
            log_error("Error reading client socket.");
            break;
        } else if (res == 0) {
            log_error("Lost connection from client.");
            break;
        }

        log_info("Received type: %d", msg.type);
        process_message(worker_queue, &msg, resp_handler_pid);
    }

    log_info("Stopping request handler.");

    if (kill(resp_handler_pid, SIGINT) < 0) {
        log_error("Error killing response handler.");
    } else {
        waitpid(resp_handler_pid, (int*) NULL, 0);
    }

    log_debug("Closing socket");
    if (close(client_socket) < 0) {
        log_error("Error closing client socket.");
    }

}