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
#include "db-connection.h"


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

void process_create(int db_connection, int global_ids, msg_t *msg) {
    msg->global_id = db_add_user(db_connection);
    if (msg->global_id < 0) {
        log_error("Error adding user to db.");
        msg->type = ACK_ERROR;
        return;
    }
    if (add_global_id(global_ids, msg->global_id, msg->mtype) < 0) {
        log_error("Error adding new global id %d for mtype %d.", msg->global_id, msg->mtype);
        msg->type = ACK_ERROR;
        return;
    }


    msg->type = ACK_CREATE;
}

void process_subscribe(int db_connection, int global_ids, msg_t *msg) {
    msg->global_id = msg->mtype;
    msg->mtype = get_mtype(global_ids, msg->global_id);
    if (msg->mtype < 0) {
        log_error("Error getting mtype for global id %d.", msg->global_id);
        msg->type = ACK_ERROR;
        return;
    }

    if (db_subscribe(db_connection, msg->global_id, msg->payload.topic) < 0) {
        log_error("Error subscribing user %d to topic %s", msg->global_id, msg->payload.topic);
        msg->type = ACK_ERROR;
        return;
    }

    msg->type = ACK_OK;
}

int send_to_subs(int db_connection, int global_ids, int resp_queue, msg_t msg) {
    int res = 0;
    vector subscribed;
    vector_new(&subscribed, sizeof(int), NULL);
    if (db_get_subscribed(db_connection, msg.payload.topic, &subscribed) < 0) {
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
        // TODO change to say if we are sending to next server instead
        log_info("Sending message to subscribed user %d", sub_id);
        if (sendmsg(resp_queue, &msg, sizeof(msg_t)) < 0) {
            log_error("Error sending message to subscribed id %d", sub_id);
            res = -1;
            break;
        }
    }
    vector_destroy(&subscribed);
    return res;

}

// Returns if the response message should be sent to the resp handler.
bool process_publish(int db_connection, int global_ids, int resp_queue, msg_t *msg) {

    log_info("Message received: %s : %s", msg->payload.topic, msg->payload.msg);

    if (msg->broker_id == BROKER_ID) {
        log_debug("Ring message back to original broker. Discarding...");
        return false;
    } else if (msg->broker_id == 0) {
        log_debug("Received publish for first time.");
        msg->broker_id = BROKER_ID;
    } else {
        log_debug("Received publish from previous ring server.");
    }

    msg->global_id = msg->mtype;
    msg->mtype = get_mtype(global_ids, msg->global_id);
    if (msg->mtype < 0) {
        log_error("Error getting mtype for global id %d.", msg->global_id);
        msg->type = ACK_ERROR;
        return true;
    }

    // We send the msg (not a pointer) so as to not modify this one, which already has the global id of the sender.
    if (send_to_subs(db_connection, global_ids, resp_queue, *msg) < 0) {
        log_error("Error sending message to subscribers.");
        msg->type = ACK_ERROR;
        return true;
    }

    msg->type = ACK_OK;
    return true;
}

void process_destroy(int db_connection, int global_ids, msg_t *msg) {
    msg->type = ACK_DESTROY;

    msg->global_id = msg->mtype;
    msg->mtype = get_mtype(global_ids, msg->global_id);
    if (msg->mtype < 0) {
        log_error("Error getting mtype for global id %d.", msg->global_id);
        msg->type = ACK_ERROR;
    }

    if (db_delete(db_connection, msg->global_id) < 0) {
        log_error("Error deleting client from DB.");
        msg->type = ACK_ERROR;
    }

    if (remove_global_id(global_ids, msg->global_id) < 0) {
        log_error("Error deleting global id %d.", msg->global_id);
        msg->type = ACK_ERROR;
    }

}

void process_msg(int db_connection, int global_ids, int resp_queue, msg_t *msg) {
    log_info("Message received of type %d", msg->type);
    bool send_response = true;
    if (msg->type == CREATE) {
        process_create(db_connection, global_ids, msg);
    } else if (msg->type == SUBSCRIBE) {
        process_subscribe(db_connection, global_ids, msg);
    } else if (msg->type == PUBLISH) {
        send_response = process_publish(db_connection, global_ids, resp_queue, msg);
    } else if (msg->type == DESTROY) {
        process_destroy(db_connection, global_ids, msg);
    } else {
        log_error("Unexpected msg type %d", msg->type);
        msg->type = ACK_ERROR;
    }

    if (send_response) {
        log_info("Sending response message to response handler.");
        if (sendmsg(resp_queue, msg, sizeof(msg_t)) < 0) {
            log_error("Error sending response message.");
        }
    }

}

int main(int argc, char* argv[]) {
    register_handler(SIGINT_handler);

    log_info("Starting worker");

    int worker_queue = get_worker_queue();
    int resp_queue = get_resp_queue();
    int global_ids = global_ids_get();
    int db_connection = db_connect();

    while(!graceful_quit) {
        log_info("Waiting request messages from handler...");
        msg_t msg;
        rcvmsg(worker_queue, &msg, sizeof(msg_t), 0);
        if (graceful_quit) {
            break;
        }
        process_msg(db_connection, global_ids, resp_queue, &msg);
    }

    log_info("Stopping worker.");
    log_debug("Closing DB connection.");
    db_disconnect(db_connection);

}