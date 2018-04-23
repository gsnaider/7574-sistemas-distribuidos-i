//
// Created by gaston on 22/04/18.
//

#include <unistd.h>
#include "../common/ipc/socket.h"
#include "server.h"
#include "../common/log/log.h"

int main(int argc, char* argv[]) {
    log_info("Server started.");
    int socket = create_server_socket(PORT);

    log_info("Waiting connections.");
    int client_socket = accept_client(socket);
    log_info("Client connected.");
    // TODO fork and process, loop back to accept.
    close(client_socket);
    close(socket);
}