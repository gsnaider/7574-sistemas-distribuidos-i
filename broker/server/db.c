//
// Created by gaston on 07/05/18.
//

#include <stdlib.h>
#include "db.h"
#include "../common/log/log.h"

int db_add_user(int id) {
    log_debug("Adding user %d to DB.", id);
    //TODO
    return 0;
}

int db_subscribe(int id, char* topic){
    log_debug("Subscribing user %d to topic '%s'.", id, topic);
    //TODO
    return 0;
}

// Returns list of ids (ints)
int db_get_subscribed(char* topic, vector* subscribed){
    log_debug("Getting subscribed users to topic '%s'.", topic);
    //TODO add subscribed users to list.
    return 0;
}

int db_delete(int id) {
    log_debug("Deleting user %d from DB.", id);
    //TODO
    return 0;
}