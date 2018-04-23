//
// Created by gaston on 22/04/18.
//
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <netdb.h>
#include <strings.h>
#include "socket.h"
#include "../log/log.h"

int create_client_socket(char* server_ip, int server_port) {
    int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_fd < 0) {
        log_error("Error creating client socket.");
        exit(-1);
    }

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(server_port);

    struct hostent* server = gethostbyname (server_ip) ;
    if (server == NULL) {
        log_error("Error calling gethostbyname.");
        exit(-1);
    }

    bcopy (server->h_addr,
           &(server_addr.sin_addr.s_addr),
           server->h_length);

    if (connect(socket_fd, (struct sockaddr*) &server_addr, sizeof(server_addr))) {
        log_error("Error calling connect.");
        exit(-1);
    }

    return socket_fd;
}

int create_server_socket(int port) {
    int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_fd < 0) {
        log_error("Error creating server socket.");
        exit(-1);
    }

    struct sockaddr_in myaddr;
    myaddr.sin_family = AF_INET;
    myaddr.sin_port = htons(port);
    myaddr.sin_addr.s_addr = INADDR_ANY;

    socklen_t myaddr_size = sizeof(myaddr);

    if (bind(socket_fd, (struct sockaddr*) &myaddr, myaddr_size ) < 0) {
        log_error("Error calling bind.");
        exit(-1);
    }

    if (listen(socket_fd, 10) < 0) {
        log_error("Error calling listen.");
        exit(-1);
    }

    return socket_fd;

}

int accept_client(int server_socket) {
    struct sockaddr_in client_addr;
    socklen_t client_addr_size = sizeof(client_addr);
    int client_fd = accept(server_socket, (struct sockaddr*) &client_addr, &client_addr_size);
    if (client_fd < 0) {
        log_error("Error accepting client.");
        exit(-1);
    }
    return client_fd;
}