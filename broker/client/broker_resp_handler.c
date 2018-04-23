//
// Created by gaston on 22/04/18.
//


#include <stdlib.h>
#include <unistd.h>
#include "../common/log/log.h"
#include "../common/ipc/msg_queue.h"
#include "broker_handler.h"

int init_msg_queue();

int main(int argc, char* argv[]) {
    log_info("Starting local broker Response Handler.");

    int resp_queue = init_msg_queue();

    log_debug("Deleting message queue");
    delmsg(resp_queue);
}

int init_msg_queue() {

    if (creamsg(BROKER_RESP_MSG) < 0) {
        log_error("Error creating resp msg queue.");
        exit(-1);
    }
    int resp_queue = getmsg(BROKER_RESP_MSG);
    if (resp_queue < 0) {
        log_error("Error getting resp message queue");
        exit(-1);
    }
    return resp_queue;

}