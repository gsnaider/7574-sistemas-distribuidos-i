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
#include "../common/ipc/semaphore.h"
#include "db_server.h"

#define BASE_DB_DIR "../db/"
#define USERS_DIR "../db/users/"
#define TOPICS_DIR "../db/topics/"
#define TMP_FILE "../db/topics/topic_tmp"
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
        log_debug("File '%s' does not exist. Creating it.", path);
        return create_file(path);
    } else {
        log_debug("File '%s' already exists.", path);
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

static void get_topic_path(char *topic, char *path) {
    strcpy(path, TOPICS_DIR);
    strcat(path, topic);
}

static void get_user_path(int id, char *path) {
    char id_str[INT_MAX_LENGTH];
    snprintf(id_str, INT_MAX_LENGTH, "%d", id);
    strcpy(path, USERS_DIR);
    strcat(path, id_str);
}

static int append_to_fd(FILE* file, char* str) {
    if (fputs(str, file) < 0) {
        return -1;
    }

    if (fputc('\n', file) < 0) {
        return -1;
    }
    return 0;
}

static int append_to_file(char *path, char *str) {
    log_debug("Appending '%s' to file '%s'", str, path);

    FILE *file = fopen(path, "a");
    if (file == NULL) {
        log_error("Error opening file '%s'", path);
        return -1;
    }

    if (append_to_fd(file, str) < 0) {
        log_error("Error writing '%s' to file '%s'", str, path);
        fclose(file);
        return -1;
    }

    fclose(file);
    return 0;
}

static int get_next_id() {
    int max_id = 0;
    DIR *dir;
    struct dirent *ent;
    if ((dir = opendir (USERS_DIR)) != NULL) {
        while ((ent = readdir (dir)) != NULL) {
            log_debug("Directory found: '%s'", ent->d_name);
            int sub_id = (int) strtol(ent->d_name, (char **)NULL, 10);
            if (sub_id > max_id) {
                max_id = sub_id;
            }
        }
        closedir (dir);
    } else {
        log_error("Error opening users directory.");
        return -1;
    }
    return max_id + 1;
}

int db_add_user() {
    log_info("Adding user to DB.");

    log_debug("Obtaining DB sem.");
    int sem = getsem(DB_SEM);
    if (sem < 0) {
        log_error("Error getting DB sem.");
        return -1;
    }

    p(sem);

    if (create_users_dir_if_not_exists() < 0) {
        v(sem);
        return -1;
    }

    int id = get_next_id();
    if (id < 0) {
        log_error("Error generating new user id.");
        v(sem);
        return -1;
    }
    log_info("New user id: '%d'", id);

    char path[strlen(USERS_DIR) + INT_MAX_LENGTH];
    get_user_path(id, path);
    if (create_file(path) < 0) {
        v(sem);
        return -1;
    }

    v(sem);

    log_debug("User '%d' added to DB.", id);
    return id;
}

int db_subscribe(int id, char *topic) {
    log_info("Subscribing user '%d' to topic '%s'.", id, topic);

    log_debug("Obtaining DB sem.");
    int sem = getsem(DB_SEM);
    if (sem < 0) {
        log_error("Error getting DB sem.");
        return -1;
    }

    p(sem);

    if (create_topics_dir_if_not_exists() < 0) {
        v(sem);
        return -1;
    }

    char user_path[strlen(USERS_DIR) + INT_MAX_LENGTH];
    get_user_path(id, user_path);
    if (!file_exists(user_path)) {
        log_error("User file '%s' does not exist.", user_path);
        v(sem);
        return -1;
    }

    char topic_path[strlen(TOPICS_DIR) + strlen(topic) + 1];
    get_topic_path(topic, topic_path);
    if (create_file_if_not_exists(topic_path) < 0) {
        v(sem);
        return -1;
    }

    char id_str[INT_MAX_LENGTH];
    snprintf(id_str, INT_MAX_LENGTH, "%d", id);

    if (append_to_file(user_path, topic) < 0) {
        v(sem);
        return -1;
    }
    if (append_to_file(topic_path, id_str) < 0) {
        v(sem);
        return -1;
    }

    v(sem);

    log_debug("User '%d' subscribed to topic '%s'.", id, topic);
    return 0;
}

// TODO check bug when getting subs from empty topic. Maybe delete empty topics when deleting users.
int db_get_subscribed(char *topic, vector *subscribed) {
    log_debug("Getting subscribed users to topic '%s'.", topic);

    log_debug("Obtaining DB sem.");
    int sem = getsem(DB_SEM);
    if (sem < 0) {
        log_error("Error getting DB sem.");
        return -1;
    }

    p(sem);


    char topic_path[strlen(TOPICS_DIR) + strlen(topic) + 1];
    get_topic_path(topic, topic_path);
    if (!file_exists(topic_path)) {
        log_info("No subscribed users for topic '%s'.", topic);
        v(sem);
        return 0;
    }

    log_debug("Opening file '%s' for reading.", topic_path);
    FILE* file = fopen(topic_path, "r");
    if (file == NULL) {
        log_error("Error opening file '%s' for reading.", topic_path);
        v(sem);
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

    v(sem);

    return 0;
}

static int unsubscribe(int id, char *topic) {
    log_info("Unsubscribing user '%d' from topic '%s'", id, topic);

    char topic_path[strlen(TOPICS_DIR) + strlen(topic) + 1];
    get_topic_path(topic, topic_path);
    if (!file_exists(topic_path)) {
        log_error("Topic file '%s' does not exist.", topic_path);
        return -1;
    }

    log_debug("Opening file '%s' for reading.", topic_path);
    FILE* file = fopen(topic_path, "r");
    log_debug("Opening file '%s' for writing.", TMP_FILE);
    FILE* out_file = fopen(TMP_FILE, "w+");

    if (file == NULL) {
        log_error("Error opening file '%s' for reading.", topic_path);
        return -1;
    }
    if (out_file == NULL) {
        log_error("Error opening file '%s' for reading.", TMP_FILE);
        return -1;
    }

    char* line = NULL;
    ssize_t chars_read;
    size_t len = 0;
    while ((chars_read = getline(&line, &len, file)) != -1) {
        int sub_id = (int) strtol(line, (char **)NULL, 10);
        if (sub_id != id) {
            //Remove trailing newline char
            if (line[chars_read - 1] == '\n') {
                line[chars_read - 1] = '\0';
            }
            if (append_to_fd(out_file, line) < 0) {
                log_error("Error appending '%s' to file '%s'", line, TMP_FILE);
                fclose(file);
                fclose(out_file);
                return -1;
            }
        }
    }
    fclose(file);
    fclose(out_file);
    if (line) {
        free(line);
    }
    if (rename(TMP_FILE, topic_path) < 0) {
        log_error("Error renaming file '%s' to '%s'", TMP_FILE, topic_path);
        return -1;
    }

    return 0;
}

int db_delete(int id) {
    log_info("Deleting user '%d' from DB.", id);

    log_debug("Obtaining DB sem.");
    int sem = getsem(DB_SEM);
    if (sem < 0) {
        log_error("Error getting DB sem.");
        return -1;
    }

    p(sem);

    char user_path[strlen(USERS_DIR) + INT_MAX_LENGTH];
    get_user_path(id, user_path);
    if (!file_exists(user_path)) {
        log_error("User file '%s' does not exist.", user_path);
        v(sem);
        return -1;
    }

    log_debug("Opening file '%s' for reading.", user_path);
    FILE* file = fopen(user_path, "r");
    if (file == NULL) {
        log_error("Error opening file '%s' for reading.", user_path);
        v(sem);
        return -1;
    }

    char* topic = NULL;
    ssize_t chars_read;
    size_t len = 0;
    while ((chars_read = getline(&topic, &len, file)) != -1) {
        //Remove trailing newline char
        if (topic[chars_read - 1] == '\n') {
            topic[chars_read - 1] = '\0';
        }
        if (unsubscribe(id, topic) < 0) {
            log_error("Error unsubscribing user '%d' from topic '%s'", id, topic);
            v(sem);
            return -1;
        }
    }
    fclose(file);
    if (topic) {
        free(topic);
    }

    log_debug("Deleting file '%s'", user_path);
    if (unlink(user_path) < 0) {
        log_error("Error deleting file '%s'", user_path);
        v(sem);
        return -1;
    }

    v(sem);

    return 0;
}