#include <unistd.h>
#include <memory.h>
#include "broker.h"
#include "broker_handler.h"
#include "../common/ipc/msg_queue.h"
#include "../common/log/log.h"

int send(msg_t* msg) {
    int req_queue = getmsg(BROKER_REQ_MSG);
    return sendmsg(req_queue, msg, sizeof(msg_t));
}

int recv(int id, msg_t* msg) {
    int resp_queue = getmsg(BROKER_RESP_MSG);
    rcvmsg(resp_queue, msg, sizeof(msg_t), id);
}

int creabrk() {
    msg_t msg;
    msg.type = CREATE;

    log_info("Sending create message.");
    send(&msg);

    log_info("Waiting response.");
    //TODO(optional): We are assuming that the mtype of the response (AKA the local id) will be the pid of the client.
    // Maybe we can do this more generically, and have the broker reply with our local id (maybe through a special queue for creation responses)
    recv(getpid(), &msg);

    if (msg.type == ACK_OK) {
        log_info("Creation successful.");
        // It's ok to return mtype (long), since mtype will be the client's pid, which is an int.
        return msg.mtype;
    } else if (msg.type == ACK_ERROR) {
        log_error("Error on creation.");
        return -1;
    } else {
        log_error("Unexpected msg type received: %d", msg.type);
        return -1;
    }
}

int publish(int id, char *message, char *topic) {
    msg_t msg;
    msg.mtype = id;
    msg.type = PUBLISH;

    strncpy(msg.payload.msg, message, MAX_MSG_LENGTH - 1);
    msg.payload.msg[MAX_MSG_LENGTH - 1] = 0; //Add null pointer terminator.

    strncpy(msg.payload.topic, topic, MAX_TOPIC_LENGTH - 1);
    msg.payload.topic[MAX_TOPIC_LENGTH - 1] = 0; //Add null pointer terminator.

    log_info("Sending publish message. Msg: '%s', topic: '%s'.", msg.payload.msg, msg.payload.topic);
    send(&msg);
    log_info("Waiting response.");
    recv(id, &msg);


    if (msg.type == ACK_OK) {
        log_info("Publish successful.");
        return 0;
    } else if (msg.type == ACK_ERROR) {
        log_error("Error on publish.");
        return -1;
    } else {
        log_error("Unexpected msg type received: %d", msg.type);
        return -1;
    }

}

int subscribe(int id, char *topic) {
    msg_t msg;
    msg.mtype = id;
    msg.type = SUBSCRIBE;

    strncpy(msg.payload.topic, topic, MAX_TOPIC_LENGTH - 1);
    msg.payload.topic[MAX_TOPIC_LENGTH - 1] = 0; //Add null pointer terminator.

    log_info("Sending subscribe message. Topic: '%s'.", msg.payload.topic);
    send(&msg);
    log_info("Waiting response.");
    recv(id, &msg);


    if (msg.type == ACK_OK) {
        log_info("Subscribe successful.");
        return 0;
    } else if (msg.type == ACK_ERROR) {
        log_error("Error on subscribe.");
        return -1;
    } else {
        log_error("Unexpected msg type received: %d", msg.type);
        return -1;
    }
}

int receive(int id, payload_t* payload) {
    msg_t msg;
    msg.mtype = id;
    msg.type = RECEIVE;

    log_info("Sending receive message.");
    send(&msg);
    log_info("Waiting response.");
    recv(id, &msg);


    if (msg.type == ACK_OK) {
        log_info("Receive successful.");
        *payload = msg.payload;
        return 0;
    } else if (msg.type == ACK_ERROR) {
        log_error("Error on receive.");
        return -1;
    } else {
        log_error("Unexpected msg type received: %d", msg.type);
        return -1;
    }
}

int delbrk(int id) {
    msg_t msg;
    msg.mtype = id;
    msg.type = DESTROY;

    log_info("Sending destroy message.");
    send(&msg);
    log_info("Waiting response.");
    recv(id, &msg);

    if (msg.type == ACK_OK) {
        log_info("Destroy successful.");
        return 0;
    } else if (msg.type == ACK_ERROR) {
        log_error("Error on destroy.");
        return -1;
    } else {
        log_error("Unexpected msg type received: %d", msg.type);
        return -1;
    }
}