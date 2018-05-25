//
// Created by gaston on 22/04/18.
//

#ifndef BROKER_SOCKET_H
#define BROKER_SOCKET_H

#include "../message.h"
#include "../../db-server/db_message.h"

int create_client_socket(char *server_ip, int server_port);

int create_server_socket(int port);

int accept_client(int server_socket);

int socket_send(int socket, msg_t *msg);

int socket_receive(int socket, msg_t *msg);

int socket_send_db(int socket, db_msg_t *msg);

int socket_receive_db(int socket, db_msg_t *msg);

#endif //BROKER_SOCKET_H
