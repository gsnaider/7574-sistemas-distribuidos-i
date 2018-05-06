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
#include "global_id.h"


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

int generate_global_id() {
    //TODO
    return 23;
}

void process_create(int global_ids, msg_t *msg) {
    msg->global_id = generate_global_id();
    if (add_global_id(global_ids, msg->mtype, msg->global_id) < 0) {
        log_error("Error adding new global id %d with mtype %d.", msg->global_id, msg->mtype);
        msg->type = ACK_ERROR;
        return;
    }
    //TODO create client in DB.
    msg->type = ACK_CREATE;
}

void process_subscribe(int global_ids, msg_t *msg) {
    msg->global_id = msg->mtype;
    //TODO subscribe client in DB.
    msg->mtype = get_mtype(global_ids, msg->global_id);
    if (msg->mtype < 0) {
        log_error("Error getting mtype for global id %d.", msg->global_id);
        msg->type = ACK_ERROR;
        return;
    }
    msg->type = ACK_OK;
}

void process_publish(int global_ids, int resp_queue, msg_t *msg) {
    msg->global_id = msg->mtype;
    //TODO search subscribed clients in DB and forward msg.

    msg->mtype = get_mtype(global_ids, msg->global_id);
    if (msg->mtype < 0) {
        log_error("Error getting mtype for global id %d.", msg->global_id);
        msg->type = ACK_ERROR;
        return;
    }

    //TODO test remove
    sendmsg(resp_queue, msg, sizeof(msg_t));

    msg->type = ACK_OK;
}

void process_destroy(int global_ids, msg_t *msg) {
    msg->global_id = msg->mtype;
    // TODO delete client in DB.

    msg->mtype = get_mtype(global_ids, msg->global_id);
    if (msg->mtype < 0) {
        log_error("Error getting mtype for global id %d.", msg->global_id);
        msg->type = ACK_ERROR;
        return;
    }
    if (remove_global_id(global_ids, msg->global_id) < 0) {
        log_error("Error deleting global id %d.", msg->global_id);
        msg->type = ACK_ERROR;
        return;
    }
    msg->type = ACK_DESTROY;
}

void process_msg(int global_ids, int resp_queue, msg_t *msg) {
    log_info("Message received of type %d", msg->type);
    if (msg->type == CREATE) {
        process_create(global_ids, msg);
    } else if (msg->type == SUBSCRIBE) {
        process_subscribe(global_ids, msg);
    } else if (msg->type == PUBLISH) {
        process_publish(global_ids, resp_queue, msg);
    } else if (msg->type == DESTROY) {
        process_destroy(global_ids, msg);
    } else {
        log_error("Unexpected msg type %d", msg->type);
        msg->type = ACK_ERROR;
    }

    log_info("Sending response message to response handler.");
    if (sendmsg(resp_queue, msg, sizeof(msg_t)) < 0) {
        log_error("Error sending response message.");
    }
}

int main(int argc, char* argv[]) {
    register_handler(SIGINT_handler);

    log_info("Starting worker");

    int worker_queue = get_worker_queue();
    int resp_queue = get_resp_queue();
    int global_ids = global_ids_get();

    while(!graceful_quit) {
        log_info("Waiting request messages from handler...");
        msg_t msg;
        rcvmsg(worker_queue, &msg, sizeof(msg_t), 0);
        if (graceful_quit) {
            break;
        }
        process_msg(global_ids, resp_queue, &msg);
    }

    log_info("Stopping worker.");
}