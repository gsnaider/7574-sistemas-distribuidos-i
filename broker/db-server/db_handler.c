//
// Created by gaston on 25/05/18.
//


#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include "../common/ipc/sig.h"
#include "../common/log/log.h"
#include "db_message.h"
#include "../common/ipc/socket.h"
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

void safe_exit(int client_socket, int error) {
    log_debug("Closing socket");
    if (close(client_socket) < 0) {
        log_error("Error closing client socket.");
    }
    exit(error);
}

void process_create(db_msg_t *msg) {
    int global_id = db_add_user();
    if (global_id < 0) {
        log_error("Error creating user on DB.");
        msg->type = DB_ERROR;
    } else {
        log_info("User %d created on DB.", global_id);
        msg->type = DB_OK;
        msg->global_id = global_id;
    }
}

void process_subscribe(db_msg_t *msg) {
    if (db_subscribe(msg->global_id, msg->topic) < 0) {
        log_error("Error subscribing user %d to topic %s", msg->global_id, msg->topic);
        msg->type = DB_ERROR;
    } else {
        log_info("User %d subscribed to topic %s", msg->global_id, msg->topic);
        msg->type = DB_OK;
    }
}

void process_get_subscribed(db_msg_t *msg) {
    vector subscribed;
    vector_new(&subscribed, sizeof(int), NULL);
    if (db_get_subscribed(msg->topic, &subscribed) < 0) {
        vector_destroy(&subscribed);
        log_error("Error getting subscribed users to topic '%s'.", msg->topic);
        msg->type = DB_ERROR;
    } else {
        for (int i = 0; i < vector_size(&subscribed) && i < MAX_CLIENTS; i++) {
            int sub_id;
            vector_item_at(&subscribed, i, &sub_id);
            msg->sub_ids[i] = sub_id;
            msg->sub_count = i + 1;
        }
        msg->type = DB_OK;
    }
}

void process_delete(db_msg_t *msg) {
    if (db_delete(msg->global_id) < 0) {
        log_error("Error deleting user %d from DB.", msg->global_id);
        msg->type = DB_ERROR;
    } else {
        log_info("User %d deleted from DB.", msg->global_id);
        msg->type = DB_OK;
    }
}

void process_message(int client_socket, db_msg_t *msg) {
    log_info("Received type: %d", msg->type);
    if (msg->type == DB_CREATE) {
        process_create(msg);
    } else if (msg->type == DB_SUBSCRIBE) {
        process_subscribe(msg);
    } else if (msg->type == DB_GET_SUBSCRIBED) {
        process_get_subscribed(msg);
    } else if (msg->type == DB_DELETE) {
        process_delete(msg);
    } else {
        log_error("Unexpected msg type received %d", msg->type);
        msg->type = DB_ERROR;
    }
    if (socket_send_db(client_socket, msg) < 0) {
        log_error("Error sending response to client.");
    }

}

int main(int argc, char* argv[]) {
    register_handler(SIGINT_handler);
    log_info("Starting DB handler.");

    if (argc < 2) {
        log_error("No client socket parameter received.");
        exit(-1);
    }
    int client_socket;
    sscanf(argv[1], "%d", &client_socket);

    db_msg_t msg;
    while(!graceful_quit) {
        log_info("Waiting for messages from client...");
        int res = socket_receive_db(client_socket, &msg);
        if(graceful_quit) {
            safe_exit(client_socket, -1);
        }
        if (res < 0) {
            log_error("Error reading client socket.");
            safe_exit(client_socket, -1);
        } else if (res == 0) {
            log_error("Lost connection from client.");
            safe_exit(client_socket, -1);
        }

        process_message(client_socket, &msg);
    }

    log_info("Closing DB handler.");
    if (close(client_socket) < 0) {
        log_error("Error closing DB client socket");
    }

}