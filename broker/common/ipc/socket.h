//
// Created by gaston on 22/04/18.
//

#ifndef BROKER_SOCKET_H
#define BROKER_SOCKET_H

int create_client_socket(char* server_ip, int server_port);
int create_server_socket(int port);
int accept_client(int server_socket);

#endif //BROKER_SOCKET_H
