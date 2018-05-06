//
// Created by gaston on 06/05/18.
//

#ifndef BROKER_GLOBAL_ID_H
#define BROKER_GLOBAL_ID_H

#include "server.h"


typedef struct global_id {
    int mtype;
    int global_id;
} global_id_t;


typedef struct broker_ids {
    int count;
    global_id_t ids[MAX_CLIENTS];
} global_ids_t;

int global_ids_create();

int global_ids_get();

int add_id(int global_ids, int mtype, int global_id);

int get_mtype(int global_ids, int global_id);

int remove_id(int global_ids, int global_id);

int global_ids_destroy(int id);

#endif //BROKER_GLOBAL_ID_H
