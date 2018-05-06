//
// Created by gaston on 06/05/18.
//

#include <stdlib.h>
#include "global_id.h"
#include "../common/ipc/semaphore.h"
#include "../common/log/log.h"
#include "../common/ipc/shm.h"

#define GLOBAL_IDS_SHM 8
#define NEXT_GLOBAL_ID_SHM 9
#define GLOBAL_IDS_SEM 10

#define INITIAL_ID 1

int global_ids_create() {
    int sem = creasem(GLOBAL_IDS_SEM);
    if (sem < 0) {
        log_error("Error creating global ids sem.");
        exit(-1);
    }
    inisem(sem, 1);

    int next_id_shm = creashm(NEXT_GLOBAL_ID_SHM, sizeof(int));
    if (next_id_shm < 0) {
        log_error("Error creating next global id shm.");
        exit(-1);
    }
    int* next_id = shm_map(next_id_shm);
    *next_id = INITIAL_ID;
    shm_unmap(next_id);

    int ids_shm = creashm(GLOBAL_IDS_SHM, sizeof(global_ids_t));
    if (ids_shm < 0) {
        log_error("Error creating global ids shm.");
        exit(-1);
    }
    return ids_shm;
}

int global_ids_get() {
    int ids_shm = getshm(GLOBAL_IDS_SHM);
    if (ids_shm < 0) {
        log_error("Error getting global ids shm.");
        exit(-1);
    }
    return ids_shm;
}

static int next_global_id(int* next_id) {
    int id = *next_id;
    *next_id = id + 1;
    return id;
}

int add_global_id(int global_ids, int mtype) {
    log_debug("Attempting to add global id with mtype %d.", mtype);
    int sem = getsem(GLOBAL_IDS_SEM);
    if (sem < 0) {
        log_error("Error getting global ids sem.");
        return -1;
    }

    int next_id_shm = getshm(NEXT_GLOBAL_ID_SHM);
    if (next_id_shm < 0) {
        log_error("Error getting next global id shm.");
        return -1;
    }
    int* next_id = shm_map(next_id_shm);

    global_ids_t* ids = shm_map(global_ids);

    p(sem);

    if (ids->count >= MAX_CLIENTS) {
        log_warn("Global clients capacity reached. Can't add more clients.");
        return -1;
    }

    global_id_t id;
    id.global_id = next_global_id(next_id);
    id.mtype = mtype;
    ids->ids[ids->count] = id;
    ids->count = ids->count + 1;

    log_debug("Global id %d added with mtype %d", id.global_id, id.mtype);

    v(sem);

    shm_unmap(ids);

    return id.global_id;
}

int get_mtype(int global_ids, int global_id) {
    log_debug("Searching mtype from global id %d.", global_id);
    int sem = getsem(GLOBAL_IDS_SEM);
    if (sem < 0) {
        log_error("Error getting global ids sem.");
        return -1;
    }

    global_ids_t* ids = shm_map(global_ids);

    p(sem);

    int found_mtype = -1;
    for (int i = 0; i < ids->count; i++) {
        global_id_t id = ids->ids[i];
        if (id.global_id == global_id) {
            found_mtype = id.mtype;
            break;
        }
    }

    v(sem);

    shm_unmap(ids);

    if (found_mtype < 0) {
        log_error("No mtype found for global_id %d.", global_id);
        return -1;
    } else {
        log_debug("mtype %d found for global id %d", found_mtype, global_id);
        return found_mtype;
    }
}

int remove_global_id(int global_ids, int global_id) {
    log_debug("Attempting to remove global id %d.", global_id);
    int sem = getsem(GLOBAL_IDS_SEM);
    if (sem < 0) {
        log_error("Error getting global ids sem.");
        return -1;
    }

    global_ids_t* ids = shm_map(global_ids);

    p(sem);

    int pos = -1;
    for (int i = 0; i < ids->count; i++) {
        global_id_t id = ids->ids[i];
        if (id.global_id == global_id) {
            pos = i;
            break;
        }
    }
    if (pos < 0) {
        log_error("No id found for global_id %d.", global_id);
    } else {
        for (int i = pos + 1; i < ids->count; i++) {
            ids->ids[i - 1] = ids->ids[i];
        }
        ids->count = ids->count - 1;
    }

    v(sem);
    shm_unmap(ids);

    return pos;
}

int global_ids_destroy(int id) {
    log_debug("Deleting global ids.");
    int res = 0;
    int sem = getsem(GLOBAL_IDS_SEM);
    if (delsem(sem) < 0) {
        log_error("Error deleting global ids sem");
        res = -1;
    }

    int next_id_shm = getshm(NEXT_GLOBAL_ID_SHM);
    if (delshm(next_id_shm) < 0) {
        log_error("Error deleting next global id shm");
        res = -1;
    }

    if (delshm(id) < 0) {
        log_error("Error deleting global ids shm");
        res = -1;
    }
    return res;
}