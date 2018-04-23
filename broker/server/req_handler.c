//
// Created by gaston on 22/04/18.
//

#include <stdlib.h>
#include <unistd.h>
#include "../common/log/log.h"

int main(int argc, char* argv[]) {
    log_info("Starting request handler.");
    if (argc < 2) {
        log_error("No client socket parameter received.");
        exit(-1);
    }
    int client_socket;
    sscanf(argv[1], "%d", &client_socket);

    char buffer[100];
    if (read(client_socket, buffer, sizeof(buffer) / sizeof(char)) < 0) {
        log_error("Error reading client socket.");
        exit(-1);
    }
    log_info("Received: %s", buffer);


    log_info("Stopping request handler.");
    if (close(client_socket) < 0) {
        log_error("Error closing client socket.");
    }

}