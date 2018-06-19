//
// Created by gaston on 07/05/18.
//

#ifndef BROKER_DB_H
#define BROKER_DB_H

#include "../common/ds/data-structures/list.h"
#include "../common/ds/data-structures/vector.h"

#define DB_IP "127.0.0.1"
#define DB_PORT 3307


int db_connect();

int db_add_user(int db_socket);

int db_subscribe(int db_socket, int id, char* topic);

int db_get_subscribed(int db_socket, char* topic, vector* subscribed);

int db_delete(int db_socket, int id);

int db_disconnect(int db_socket);

#endif //BROKER_DB_H
