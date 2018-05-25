//
// Created by gaston on 07/05/18.
//

#include <memory.h>
#include <unistd.h>
#include "db-connection.h"
#include "../common/log/log.h"
#include "../common/ipc/socket.h"

int db_connect() {
    int db_socket = create_client_socket(DB_IP, DB_PORT);
    if (db_socket < 0) {
        log_error("Error connecting to DB.");
    }
    return db_socket;
}

int db_add_user(int db_socket) {
    log_info("Adding user to DB.");

    db_msg_t msg;
    msg.type = DB_CREATE;
    if (socket_send_db(db_socket, &msg) < 0) {
        log_error("Error sending create message to DB.");
        return -1;
    }
    int res = socket_receive_db(db_socket, &msg);
    if (res < 0) {
        log_error("Error reading DB socket.");
        return -1;
    }
    if (res == 0) {
        log_error("Lost connection to DB.");
        return -1;
    }
    if (msg.type == DB_ERROR) {
        log_error("Error on create from DB.");
        return -1;
    }

    log_debug("User '%d' added to DB.", msg.global_id);
    return msg.global_id;
}

int db_subscribe(int db_socket, int id, char *topic) {
    log_info("Subscribing user '%d' to topic '%s'.", id, topic);

    db_msg_t msg;
    msg.global_id = id;
    msg.type = DB_SUBSCRIBE;

    strncpy(msg.topic, topic, MAX_TOPIC_LENGTH - 1);
    msg.topic[MAX_TOPIC_LENGTH - 1] = 0; //Add null pointer terminator.

    if (socket_send_db(db_socket, &msg) < 0) {
        log_error("Error sending subscribe message to DB.");
        return -1;
    }
    int res = socket_receive_db(db_socket, &msg);
    if (res < 0) {
        log_error("Error reading DB socket.");
        return -1;
    }
    if (res == 0) {
        log_error("Lost connection to DB.");
        return -1;
    }
    if (msg.type == DB_ERROR) {
        log_error("Error on subscribe from DB.");
        return -1;
    }

    log_debug("User '%d' subscribed to topic '%s'.", id, topic);
    return 0;
}

int db_get_subscribed(int db_socket, char *topic, vector *subscribed) {
    log_debug("Getting subscribed users to topic '%s'.", topic);

    db_msg_t msg;
    msg.type = DB_GET_SUBSCRIBED;

    strncpy(msg.topic, topic, MAX_TOPIC_LENGTH - 1);
    msg.topic[MAX_TOPIC_LENGTH - 1] = 0; //Add null pointer terminator.

    if (socket_send_db(db_socket, &msg) < 0) {
        log_error("Error sending subscribe message to DB.");
        return -1;
    }
    int res = socket_receive_db(db_socket, &msg);
    if (res < 0) {
        log_error("Error reading DB socket.");
        return -1;
    }
    if (res == 0) {
        log_error("Lost connection to DB.");
        return -1;
    }
    if (msg.type == DB_ERROR) {
        log_error("Error on subscribe from DB.");
        return -1;
    }

    for(int i = 0; i < msg.sub_count; i++) {
        vector_add(subscribed, &(msg.sub_ids[i]));
    }

    return 0;
}

int db_delete(int db_socket, int id) {
    log_info("Deleting user '%d' from DB.", id);

    db_msg_t msg;
    msg.global_id = id;
    msg.type = DB_DELETE;

    if (socket_send_db(db_socket, &msg) < 0) {
        log_error("Error sending subscribe message to DB.");
        return -1;
    }
    int res = socket_receive_db(db_socket, &msg);
    if (res < 0) {
        log_error("Error reading DB socket.");
        return -1;
    }
    if (res == 0) {
        log_error("Lost connection to DB.");
        return -1;
    }
    if (msg.type == DB_ERROR) {
        log_error("Error on subscribe from DB.");
        return -1;
    }

    return 0;
}

int db_disconnect(int db_socket) {
    if (close(db_socket) < 0) {
        log_error("Error disconnecting from DB.");
    }
    return 0;
}