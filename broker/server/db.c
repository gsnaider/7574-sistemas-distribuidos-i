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

#define BASE_DB_DIR "../db/"
#define USERS_DIR "../db/users/"
#define TOPICS_DIR "../db/topics/"
#define INT_MAX_LENGTH 12


static bool file_exists(char *path) {
    return (access(path, F_OK) != -1);
}

static int create_file(char *path) {
    FILE *file = fopen(path, "a");
    if (file == NULL) {
        log_error("Error creating file '%s'.", path);
        return -1;
    } else {
        log_debug("File created '%s'", path);
        fclose(file);
        return 0;
    }

}

static int create_file_if_not_exists(char *path) {
    if (!file_exists(path)) {
        log_debug("File %s does not exist. Creating it.", path);
        return create_file(path);
    } else {
        log_debug("File %s already exists.", path);
        return 0;
    }
}

static int create_dir_if_not_exists(char *dir_path) {
    DIR *dir = opendir(dir_path);
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

void get_topic_path(char *topic, char *path) {
    strcpy(path, TOPICS_DIR);
    strcat(path, topic);
}

void get_user_path(int id, char *path) {
    char id_str[INT_MAX_LENGTH];
    snprintf(id_str, INT_MAX_LENGTH, "%d", id);
    strcpy(path, USERS_DIR);
    strcat(path, id_str);
}


static int append_to_file(char *path, char *str) {
    log_debug("Appending '%s' to file '%s'", str, path);

    FILE *file = fopen(path, "a");
    if (file == NULL) {
        log_error("Error opening file '%s'", path);
        return -1;
    }

    if (fputs(str, file) < 0) {
        log_error("Error writing '%s' to file '%s'", str, path);
        fclose(file);
        return -1;
    }

    if (fputc('\n', file) < 0) {
        log_error("Error writing '\\n' to file '%s'", path);
        fclose(file);
        return -1;
    }

    fclose(file);
    return 0;
}

int db_add_user(int id) {
    log_info("Adding user %d to DB.", id);
    if (create_users_dir_if_not_exists() < 0) {
        return -1;
    }
    char path[strlen(USERS_DIR) + INT_MAX_LENGTH];
    get_user_path(id, path);
    if (create_file(path) < 0) {
        return -1;
    }
    log_debug("User %d added to DB.", id);
    return 0;
}

int db_subscribe(int id, char *topic) {
    log_info("Subscribing user %d to topic '%s'.", id, topic);
    if (create_topics_dir_if_not_exists() < 0) {
        return -1;
    }

    char user_path[strlen(USERS_DIR) + INT_MAX_LENGTH];
    get_user_path(id, user_path);
    if (!file_exists(user_path)) {
        log_error("User file %s does not exist.", user_path);
        return -1;
    }

    char topic_path[strlen(TOPICS_DIR) + strlen(topic) + 1];
    get_topic_path(topic, topic_path);
    if (create_file_if_not_exists(topic_path) < 0) {
        return -1;
    }

    char id_str[INT_MAX_LENGTH];
    snprintf(id_str, INT_MAX_LENGTH, "%d", id);

    if (append_to_file(user_path, topic) < 0) {
        return -1;
    }
    if (append_to_file(topic_path, id_str) < 0) {
        return -1;
    }

    log_debug("User %d subscribed to topic '%s'.", id, topic);
    return 0;
}

int db_get_subscribed(char *topic, vector *subscribed) {
    log_debug("Getting subscribed users to topic '%s'.", topic);

    char topic_path[strlen(TOPICS_DIR) + strlen(topic) + 1];
    get_topic_path(topic, topic_path);
    if (!file_exists(topic_path)) {
        log_info("No subscribed users for topic '%s'.", topic);
        return 0;
    }

    log_debug("Opening file %s for reading.", topic_path);
    FILE* file = fopen(topic_path, "r");
    if (file == NULL) {
        log_error("Error opening file %s for reading.", topic_path);
        return -1;
    }

    char* line = NULL;
    ssize_t read;
    size_t len = 0;
    while ((read = getline(&line, &len, file)) != -1) {
        int sub_id = (int) strtol(line, (char **)NULL, 10);
        log_info("Found id '%d' subscribed to topic '%s'", sub_id, topic);
        vector_add(subscribed, &sub_id);
    }
    fclose(file);
    if (line) {
        free(line);
    }

    return 0;
}

int db_delete(int id) {
    log_info("Deleting user %d from DB.", id);
    //TODO
    return 0;
}