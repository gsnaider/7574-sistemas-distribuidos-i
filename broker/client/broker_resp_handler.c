//
// Created by gaston on 22/04/18.
//


#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include "../common/log/log.h"
#include "../common/ipc/msg_queue.h"
#include "broker_handler.h"
#include "../common/ipc/shm.h"
#include "../common/message.h"
#include "../common/ipc/semaphore.h"
#include "../common/ipc/sig.h"
#include "../common/ipc/socket.h"
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


int get_msg_resp_queue() {
    int resp_queue = getmsg(BROKER_RESP_MSG);
    if (resp_queue < 0) {
        log_error("Error getting resp message queue");
        exit(-1);
    }
    return resp_queue;
}


int get_incoming_msg_queue() {
    int queue = getmsg(INCOMING_MSG_QUEUE);
    if (queue < 0) {
        log_error("Error getting incoming message queue");
        exit(-1);
    }
    return queue;
}


void process_publish(int incoming_msg_queue, msg_t *msg) {
    log_debug("Adding message '%s', topic '%s' to incoming messages queue.", msg->payload.msg, msg->payload.topic);
    if (sendmsg(incoming_msg_queue, msg, sizeof(msg_t)) < 0) {
        log_error("Error adding incoming message to queue.");
    }
}


void process_message(int broker_ids, int resp_queue, int incoming_msg_queue, msg_t *msg) {
    log_info("Processing message.");
    if (msg->type == PUBLISH) {
        process_publish(incoming_msg_queue, msg);
    } else {
        int local_id;
        if (msg->type == ACK_CREATE){
            msg->type = ACK_OK;
            local_id = set_global_id(broker_ids, msg->mtype);
        } else {
            local_id = get_local_id(broker_ids, msg->mtype);
            if (msg->type == ACK_OK) {
                log_debug("OK message received from server.");
            } else if (msg->type == ACK_DESTROY) {
                log_debug("OK DESTROY message received from server.");
                remove_id(broker_ids, local_id);
            } else if (msg->type == ACK_ERROR) {
                log_error("Error message received from server.");
            } else {
                log_error("Unexpected msg type received from server: %d", msg->type);
            }
        }
        msg->mtype = local_id;
        log_info("Sending message to client.");
        if (sendmsg(resp_queue, msg, sizeof(msg_t)) < 0 ) {
            log_error("Error sending message to client.");
        }
    }
}

int main(int argc, char* argv[]) {
    register_handler(SIGINT_handler);

    log_info("Starting local broker Response Handler.");

    if (argc < 2) {
        log_error("No socket parameter received.");
        exit(-1);
    }
    int socket_fd;
    sscanf(argv[1], "%d", &socket_fd);

    int resp_queue = get_msg_resp_queue();
    int incoming_msg_queue = get_incoming_msg_queue();
    int broker_ids = broker_id_get();

    while (!graceful_quit) {
        msg_t msg;
        log_info("Waiting responses from server...");
        int res = socket_receive(socket_fd, &msg);
        if (graceful_quit) {
            break;
        }
        if (res < 0) {
            log_error("Error reading socket.");
            // TODO(optional) kill broker req handler
            break;
        } else if (res == 0) {
            log_error("Lost connection from server.");
            // TODO(optional) kill broker req handler
            break;
        }

        log_info("Received message of type: %d", msg.type);
        process_message(broker_ids, resp_queue, incoming_msg_queue, &msg);

    }

    log_info("Stopping response handler.");

    log_debug("Closing socket");
    close(socket_fd);


}