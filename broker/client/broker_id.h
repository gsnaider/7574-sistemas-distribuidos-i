#ifndef BROKER_ID_H
#define BROKER_ID_H

#include <stdbool.h>

typedef struct broker_id {
    int local_id;
    int global_id;
} broker_id_t;

int broker_id_create();

int broker_id_get();

// Adds the local_id to the broker_ids, without a specifying a global_id yet.
int add_local_id(int broker_ids, int local_id);

// searches in broker_ids for any local_id without global_id, and assigns the global id to it.
// Returns the local_id to which global_id was assigned.
int add_global_id(int broker_ids, int global_id);

int get_global_id(int broker_ids, int local_id);

int get_local_id(int broker_ids, int global_id);

bool local_id_exists(int broker_ids, int local_id);

int broker_id_destroy(int id);

#endif