//
// Created by gaston on 22/04/18.
//
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <netdb.h>
#include <strings.h>
#include <unistd.h>
#include <memory.h>
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
    log_debug("Creating socket on port %d", port);
    int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_fd < 0) {
        log_error("Error creating server socket.");
        return -1;
    }

    struct sockaddr_in myaddr;
    myaddr.sin_family = AF_INET;
    myaddr.sin_port = htons(port);
    myaddr.sin_addr.s_addr = INADDR_ANY;

    socklen_t myaddr_size = sizeof(myaddr);

    if (bind(socket_fd, (struct sockaddr*) &myaddr, myaddr_size ) < 0) {
        log_error("Error calling bind.");
        return -1;
    }

    if (listen(socket_fd, 10) < 0) {
        log_error("Error calling listen.");
        return -1;
    }

    return socket_fd;

}

int accept_client(int server_socket) {
    struct sockaddr_in client_addr;
    socklen_t client_addr_size = sizeof(client_addr);
    int client_fd = accept(server_socket, (struct sockaddr*) &client_addr, &client_addr_size);
    if (client_fd < 0) {
        log_error("Error accepting client.");
    }
    return client_fd;
}

int socket_send(int socket, msg_t *msg) {
    char buffer[sizeof(msg_t) / sizeof(char)];
    memcpy(buffer, msg, sizeof(msg_t));
    int bytes_sent = 0;
    while (bytes_sent < sizeof(msg_t)) {
        int bytes = write(socket, buffer + bytes_sent, sizeof(msg_t) - bytes_sent);
        if (bytes < 0) {
            log_error("Error writing to socket.");
            return -1;
        } else if (bytes == 0) {
            log_error("Lost connection.");
            return 0;
        }
        bytes_sent += bytes;
    }

    return bytes_sent;
}

int socket_receive(int socket, msg_t *msg) {
    char buffer[sizeof(msg_t) / sizeof(char)];
    int bytes_read = 0;
    while (bytes_read < sizeof(msg_t)) {
        int bytes = read(socket, buffer + bytes_read, sizeof(msg_t) - bytes_read);
        if (bytes < 0) {
            log_error("Error reading from socket.");
            return -1;
        } else if (bytes == 0) {
            log_error("Lost connection.");
            return 0;
        }
        bytes_read += bytes;
    }
    *msg = *((msg_t*) buffer);
    return bytes_read;
}


int socket_send_db(int socket, db_msg_t *msg) {
    char buffer[sizeof(db_msg_t) / sizeof(char)];
    memcpy(buffer, msg, sizeof(db_msg_t));
    int bytes_sent = 0;
    while (bytes_sent < sizeof(db_msg_t)) {
        int bytes = write(socket, buffer + bytes_sent, sizeof(db_msg_t) - bytes_sent);
        if (bytes < 0) {
            log_error("Error writing to socket.");
            return -1;
        } else if (bytes == 0) {
            log_error("Lost connection.");
            return 0;
        }
        bytes_sent += bytes;
    }

    return bytes_sent;
}

int socket_receive_db(int socket, db_msg_t *msg) {
    char buffer[sizeof(db_msg_t) / sizeof(char)];
    int bytes_read = 0;
    while (bytes_read < sizeof(db_msg_t)) {
        int bytes = read(socket, buffer + bytes_read, sizeof(db_msg_t) - bytes_read);
        if (bytes < 0) {
            log_error("Error reading from socket.");
            return -1;
        } else if (bytes == 0) {
            log_error("Lost connection.");
            return 0;
        }
        bytes_read += bytes;
    }
    *msg = *((db_msg_t*) buffer);
    return bytes_read;
}