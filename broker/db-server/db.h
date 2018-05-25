//
// Created by gaston on 07/05/18.
//

#ifndef BROKER_DB_H
#define BROKER_DB_H

#include "../common/ds/data-structures/list.h"
#include "../common/ds/data-structures/vector.h"

int db_add_user();

int db_subscribe(int id, char* topic);

int db_get_subscribed(char* topic, vector* subscribed);

int db_delete(int id);

#endif //BROKER_DB_H
