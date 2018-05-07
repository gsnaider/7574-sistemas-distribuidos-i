//
// Created by gaston on 07/05/18.
//

#include <stdlib.h>
#include <sys/stat.h>
#include <memory.h>
#include <dirent.h>
#include <errno.h>
#include "db.h"
#include "../common/log/log.h"

#define BASE_DB_DIR "../db/"
#define USERS_DIR "../db/users/"
#define TOPICS_DIR "../db/topics/"
#define ID_MAX_LENGTH 12 //max length of int

static int create_dir_if_not_exists(char* dir_path) {
    DIR* dir = opendir(dir_path);
    if (dir) {
        log_debug("Directory '%s' already exists.", dir_path);
        closedir(dir);
        return 0;
    } else if (ENOENT == errno) {
        log_debug("Directory '%s' does not exists. Creating it.", dir_path);
        if (mkdir(dir_path, 0700) < 0) {
            log_error("Error creating directory '%s'.", dir_path);
            return -1;
        }
        return 0;
    } else {
        log_error("Failed to open dir '%s'.", dir_path);
        return -1;
    }
}

static int create_users_dir_if_not_exists() {
    if (create_dir_if_not_exists(BASE_DB_DIR) < 0) {
        return -1;
    }
    if (create_dir_if_not_exists(USERS_DIR) < 0) {
        return -1;
    }
}

static int create_topics_dir_if_not_exists() {
    if (create_dir_if_not_exists(BASE_DB_DIR) < 0) {
        return -1;
    }
    if (create_dir_if_not_exists(TOPICS_DIR) < 0) {
        return -1;
    }
}

void get_user_path(int id, char* path) {
    char id_str[ID_MAX_LENGTH];
    snprintf(id_str, ID_MAX_LENGTH, "%d", id);
    strcpy(path, USERS_DIR);
    strcat(path, id_str);
}

static int create_file(char *path) {
    FILE* file = fopen(path, "a");
    if (file == NULL) {
        log_error("Error creating file '%s'.", path);
        return -1;
    } else {
        log_debug("File created '%s'", path);
        fclose(file);
        return 0;
    }
}

int db_add_user(int id) {
    log_debug("Adding user %d to DB.", id);
    if (create_users_dir_if_not_exists() < 0) {
        return -1;
    }
    char path[strlen(USERS_DIR) + ID_MAX_LENGTH];
    get_user_path(id, path);
    if (create_file(path) < 0) {
        return -1;
    }
    log_debug("User %d added to DB.", id);
    return 0;
}

int db_subscribe(int id, char* topic){
    log_debug("Subscribing user %d to topic '%s'.", id, topic);



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