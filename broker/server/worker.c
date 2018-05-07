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
#include "db.h"


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

void process_create(int global_ids, msg_t *msg) {
    msg->global_id = add_global_id(global_ids, msg->mtype);
    if (msg->global_id < 0) {
        log_error("Error adding new global id for mtype %d.", msg->mtype);
        msg->type = ACK_ERROR;
        return;
    }

    db_add_user(msg->global_id);

    msg->type = ACK_CREATE;
}

void process_subscribe(int global_ids, msg_t *msg) {
    msg->global_id = msg->mtype;
    msg->mtype = get_mtype(global_ids, msg->global_id);
    if (msg->mtype < 0) {
        log_error("Error getting mtype for global id %d.", msg->global_id);
        msg->type = ACK_ERROR;
        return;
    }

    db_subscribe(msg->global_id, msg->payload.topic);

    msg->type = ACK_OK;
}

int send_to_subs(int global_ids, int resp_queue, msg_t msg) {
    int res = 0;
    vector subscribed;
    vector_new(&subscribed, sizeof(int), NULL);
    if (db_get_subscribed(msg.payload.topic, &subscribed) < 0) {
        vector_destroy(&subscribed);
        log_error("Error getting subscribed users to topic '%s'.", msg.payload.topic);
        return -1;
    }
    for (int i = 0; i < vector_size(&subscribed); i++) {
        int sub_id;
        vector_item_at(&subscribed, i, &sub_id);
        msg.global_id = sub_id;
        msg.mtype = get_mtype(global_ids, sub_id);
        if (msg.mtype < 0) {
            log_error("Error getting mtype for global id %d.", sub_id);
            res = -1;
            break;
        }
        if (sendmsg(resp_queue, &msg, sizeof(msg_t)) < 0) {
            log_error("Error sending message to subscribed id %d", sub_id);
            res = -1;
            break;
        }
    }
    vector_destroy(&subscribed);
    return res;

}

void process_publish(int global_ids, int resp_queue, msg_t *msg) {

    msg->global_id = msg->mtype;
    msg->mtype = get_mtype(global_ids, msg->global_id);
    if (msg->mtype < 0) {
        log_error("Error getting mtype for global id %d.", msg->global_id);
        msg->type = ACK_ERROR;
        return;
    }

    // We send the msg (not a pointer) so as to not modify this one, which already has the global id of the sender.
    if (send_to_subs(global_ids, resp_queue, *msg) < 0) {
        log_error("Error sending message to subscribers.");
        msg->type = ACK_ERROR;
        return;
    }

    //TODO test remove
    sendmsg(resp_queue, msg, sizeof(msg_t));

    msg->type = ACK_OK;
}

void process_destroy(int global_ids, msg_t *msg) {
    msg->type = ACK_DESTROY;

    msg->global_id = msg->mtype;
    msg->mtype = get_mtype(global_ids, msg->global_id);
    if (msg->mtype < 0) {
        log_error("Error getting mtype for global id %d.", msg->global_id);
        msg->type = ACK_ERROR;
    }

    if (db_delete(msg->global_id) < 0) {
        log_error("Error deleting client from DB.");
        msg->type = ACK_ERROR;
    }

    if (remove_global_id(global_ids, msg->global_id) < 0) {
        log_error("Error deleting global id %d.", msg->global_id);
        msg->type = ACK_ERROR;
    }

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