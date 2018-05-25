//
// Created by gaston on 07/05/18.
//

#include <stdlib.h>
#include <sys/stat.h>
#include <memory.h>
#include <dirent.h>
#include <errno.h>
#include <unistd.h>
#include "db.h"
#include "../common/log/log.h"

int db_add_user() {
    log_info("Adding user to DB.");
    int id = 23;
    log_debug("User '%d' added to DB.", id);
    return id;
}

int db_subscribe(int id, char *topic) {
    log_info("Subscribing user '%d' to topic '%s'.", id, topic);
    log_debug("User '%d' subscribed to topic '%s'.", id, topic);
    return 0;
}

int db_get_subscribed(char *topic, vector *subscribed) {
    log_debug("Getting subscribed users to topic '%s'.", topic);
    return 0;
}

int unsubscribe(int id, char *topic) {
    log_info("Unsubscribing user '%d' from topic '%s'", id, topic);
    return 0;
}

int db_delete(int id) {
    log_info("Deleting user '%d' from DB.", id);
    return 0;
}