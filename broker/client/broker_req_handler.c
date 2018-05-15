//
// Created by gaston on 22/04/18.
//

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <memory.h>
#include <stdbool.h>
#include <sys/wait.h>
#include "../common/log/log.h"
#include "../common/ipc/msg_queue.h"
#include "broker_handler.h"
#include "../common/ipc/socket.h"
#include "../common/ipc/shm.h"
#include "../common/message.h"
#include "../common/ipc/semaphore.h"
#include "../common/ipc/sig.h"
#include "broker_id.h"

bool graceful_quit = false;

void SIGINT_handler(int signum) {
    if (signum != SIGINT) {
        log_warn("WARNING: Unknown signal received: %d.", signum);
    } else {
        log_debug("SIGINT received, aborting.");
        graceful_quit = true;
    }
}

int init_msg_req_queue() {
    int req_queue = creamsg(BROKER_REQ_MSG);
    if (req_queue < 0) {
        log_error("Error creating req msg queue.");
        exit(-1);
    }
    return req_queue;
}


int init_msg_resp_queue() {
    int resp_queue = creamsg(BROKER_RESP_MSG);
    if (resp_queue < 0) {
        log_error("Error creating resp msg queue.");
        exit(-1);
    }
    return resp_queue;
}


int init_incoming_msg_queue() {
    int queue = creamsg(INCOMING_MSG_QUEUE);
    if (queue < 0) {
        log_error("Error creating incoming msg queue.");
        exit(-1);
    }
    return queue;
}


pid_t create_broker_resp_handler(int socket_fd) {
    //TODO(optional) send req pid to resp handler, so that if there's an error he can kill the req handler.
    pid_t pid = fork();
    if (pid < 0) {
        log_error("Error forking broker resp handler");
        exit(-1);
    }
    if (pid == 0) {
        //TODO see if we need to update 3 to 12.
        char socket_str[3];
        snprintf(socket_str, 3, "%d", socket_fd);
        execl("./broker_resp_handler", "./broker_resp_handler", socket_str, (char*)NULL);
        log_error("Error executing broker resp handler");
        exit(-1);
    }
    return pid;

}

void process_receive(int resp_queue, int incoming_msg_queue, msg_t *msg) {
    log_info("Checking for incoming messages.");

    msg_t incoming_msg;
    // TODO This causes broer_req_handler to block until a new message arrives.
    // Other clients cant use broker_req_handler until it is unlocked..
    // Maybe fork another process for waiting messages.
    int res = rcvmsg(incoming_msg_queue, &incoming_msg, sizeof(msg_t), msg->mtype);
    if ( res < 0) {
        log_error("Error receiving incoming message.");
        incoming_msg.type = ACK_ERROR;
    } else {
        log_info("New message found: %s : %s", incoming_msg.payload.topic, incoming_msg.payload.msg);
        incoming_msg.type = ACK_OK;
    }

    log_info("Sending message to client %d.", incoming_msg.mtype);
    if (sendmsg(resp_queue, &incoming_msg, sizeof(msg_t)) < 0) {
        log_error("Error sending incoming message to repsonse queue.");
    }

}

void process_msg(int broker_ids, int resp_queue, int socket, int incoming_msg_queue, msg_t* msg) {
    if (msg->type != CREATE && !local_id_exists(broker_ids, msg->mtype)) {
        log_error("Invalid broker id: %d for message type %d", msg->mtype, msg->type);
        return;
    }

    if (msg->type == ACK_OK || msg->type == ACK_ERROR || msg->type == ACK_CREATE || msg->type == ACK_DESTROY) {
        log_error("Unexpected msg type received: %d", msg->type);
        return;
    }

    log_info("Message received of type %d.", msg->type);
    if (msg->type == RECEIVE) {
        process_receive(resp_queue, incoming_msg_queue, msg);
    } else {
        if (msg->type == CREATE) {
            if (add_local_id(broker_ids, msg->mtype) < 0) {
                log_error("Error adding local client.");
                return;
            }
        } else {
            msg->mtype = get_global_id(broker_ids, msg->mtype);
        }
        log_info("Sending message to server.");
        socket_send(socket, msg);
    }

}

int main(int argc, char* argv[]) {
    register_handler(SIGINT_handler);

    log_info("Starting local broker Request Handler.");

    int server_socket = create_client_socket(SERVER_IP, SERVER_PORT);
    log_info("Connected to server");

    int req_queue = init_msg_req_queue();
    int resp_queue = init_msg_resp_queue();
    int incoming_msg_queue = init_incoming_msg_queue();
    int broker_ids = broker_id_create();

    pid_t resp_handler = create_broker_resp_handler(server_socket);

    while(!graceful_quit) {
        log_info("Waiting request messages from client...");
        msg_t msg;
        rcvmsg(req_queue, &msg, sizeof(msg_t), 0);
        if (graceful_quit) {
            break;
        }
        process_msg(broker_ids, resp_queue, server_socket, incoming_msg_queue, &msg);
    }

    log_info("Stopping request handler.");

    if (kill(resp_handler, SIGINT) < 0) {
        log_error("Error killing response handler.");
    } else {
        waitpid(resp_handler, (int*) NULL, 0);
    }

    log_debug("Deleting message queues.");
    delmsg(req_queue);
    delmsg(resp_queue);
    delmsg(incoming_msg_queue);

    log_debug("Deleting broker ids");
    broker_id_destroy(broker_ids);

    log_debug("Closing socket.");
    close(server_socket);
}
