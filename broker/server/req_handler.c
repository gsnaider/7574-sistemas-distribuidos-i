//
// Created by gaston on 22/04/18.
//

#include <stdlib.h>
#include <unistd.h>
#include "../common/log/log.h"
#include "../common/message.h"

int main(int argc, char* argv[]) {
    log_info("Starting request handler.");
    if (argc < 2) {
        log_error("No client socket parameter received.");
        exit(-1);
    }
    int client_socket;
    sscanf(argv[1], "%d", &client_socket);

    // TODO read message_t instead of
    char buffer[sizeof(msg_t) / sizeof(char)];
    if (read(client_socket, buffer, sizeof(buffer) / sizeof(char)) < 0) {
        log_error("Error reading client socket.");
        exit(-1);
    }
    msg_t* msg = (msg_t*) buffer;
    log_info("Received type: %d", msg->type);


    log_info("Stopping request handler.");
    if (close(client_socket) < 0) {
        log_error("Error closing client socket.");
    }

}