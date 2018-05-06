//
// Created by gaston on 05/05/18.
//


#include <stdlib.h>
#include <stdbool.h>
#include "../common/log/log.h"
#include "../common/ipc/msg_queue.h"
#include "server.h"
#include "../common/ipc/sig.h"
#include "../common/message.h"


bool graceful_quit = false;

void SIGINT_handler(int signum) {
    if (signum != SIGINT) {
        log_warn("WARNING: Unknown signal received: %d.", signum);
    } else {
        log_debug("SIGINT received, aborting.");
        graceful_quit = true;
    }
}

int get_worker_queue() {
    int worker_queue = getmsg(WORKER_QUEUE);
    if (worker_queue < 0) {
        log_error("Error getting worker queue.");
        exit(-1);
    }
    return worker_queue;
}

int get_resp_queue() {
    int resp_queue = getmsg(RESP_QUEUE);
    if (resp_queue  < 0) {
        log_error("Error getting response queue.");
        exit(-1);
    }
    return resp_queue;
}

void process_msg(int resp_queue, msg_t *msg) {
    log_info("Message received of type %d", msg->type);
    //TODO: add real functionality, all this is just a mock
    msg->global_id = 23;
    if (msg->type == CREATE) {
        msg->type = ACK_CREATE;
    } else if (msg->type == SUBSCRIBE) {
        msg->type = ACK_OK;
    } else if (msg->type == PUBLISH) {
        // send to subscribers.
        log_info("Sending publish message to response handler.");
        if (sendmsg(resp_queue, msg, sizeof(msg_t)) < 0) {
            log_error("Error sending publish message.");
        }
        msg->type = ACK_OK;
    } else if (msg->type == DESTROY) {
        msg->type = ACK_DESTROY;
    } else {
        log_error("Unexpected msg type %d", msg->type);
        msg->type = ACK_ERROR;
    }


    log_info("Sending message to response handler.");
    if (sendmsg(resp_queue, msg, sizeof(msg_t)) < 0) {
        log_error("Error sending response message.");
    }
}

int main(int argc, char* argv[]) {
    register_handler(SIGINT_handler);

    log_info("Starting worker");

    int worker_queue = get_worker_queue();
    int resp_queue = get_resp_queue();

    while(!graceful_quit) {
        log_info("Waiting request messages from handler...");
        msg_t msg;
        rcvmsg(worker_queue, &msg, sizeof(msg_t), 0);
        if (graceful_quit) {
            break;
        }
        process_msg(resp_queue, &msg);
    }

    log_info("Stopping worker.");
}