#include <stdlib.h>
#include <unistd.h>
#include "broker_id.h"
#include "../common/log/log.h"
#include "../common/ipc/shm.h"
#include "broker_handler.h"
#include "../common/ipc/semaphore.h"

int broker_id_create() {
    int sem = creasem(BROKER_IDS_SEM);
    if (sem < 0) {
        log_error("Error creating broker ids sem.");
        exit(-1);
    }
    inisem(sem, 1);

    int ids_shm = creashm(BROKER_IDS_SHM, sizeof(broker_ids_t));
    if (ids_shm < 0) {
        log_error("Error creating broker ids shm.");
        exit(-1);
    }
    return ids_shm;
}

int broker_id_get() {
    int ids_shm = getshm(BROKER_IDS_SHM);
    if (ids_shm < 0) {
        log_error("Error getting broker ids shm.");
        exit(-1);
    }
    return ids_shm;
}

// Adds the local_id to the broker_ids, without a specifying a global_id yet.
int add_local_id(int broker_ids, int local_id) {
    log_debug("Attempting to add local id %d.", local_id);
    int sem = getsem(BROKER_IDS_SEM);
    if (sem < 0) {
        log_error("Error getting broker ids sem.");
        return -1;
    }
    p(sem);

    broker_ids_t* ids = shm_map(broker_ids);

    if (ids->count >= MAX_CLIENTS) {
        log_warn("Client capacity reached. Can't add more clients.");
        return -1;
    }

    broker_id_t id;
    id.global_id = 0; //No global id assigned yet.
    id.local_id = local_id;

    ids->ids[ids->count] = id;
    ids->count = ids->count + 1;

    shm_unmap(ids);

    v(sem);

    log_debug("Local id %d added to broker ids", local_id);
}

// searches in broker_ids for any local_id without global_id, and assigns the global id to it.
// Returns the local_id to which global_id was assigned.
int set_global_id(int broker_ids, int global_id) {
    log_debug("Attempting to assign global id %d to a local id.", global_id);
    int sem = getsem(BROKER_IDS_SEM);
    if (sem < 0) {
        log_error("Error getting broker ids sem.");
        return -1;
    }
    p(sem);

    broker_ids_t* ids = shm_map(broker_ids);

    int assigned_local_id = -1;
    for (int i = 0; i < ids->count; i++) {
        broker_id_t id = ids->ids[i];
        if (id.global_id == 0) {
            ids->ids[i].global_id = global_id;
            assigned_local_id = id.local_id;
            break;
        }
    }

    shm_unmap(ids);

    v(sem);


    if (assigned_local_id < 0) {
        log_error("No local_id found without a global_id to be assigned.");
        return -1;
    } else {
        log_debug("Global id %d set to local id %d.", global_id, assigned_local_id);
        return assigned_local_id;
    }

}


int get_global_id(int broker_ids, int local_id) {
    log_debug("Attempting to get global id for local id %d.", local_id);
    int sem = getsem(BROKER_IDS_SEM);
    if (sem < 0) {
        log_error("Error getting broker ids sem.");
        return -1;
    }
    p(sem);

    broker_ids_t* ids = shm_map(broker_ids);

    int found_global_id = -1;
    for (int i = 0; i < ids->count; i++) {
        broker_id_t id = ids->ids[i];
        if (id.local_id == local_id) {
            found_global_id = id.global_id;
            break;
        }
    }

    shm_unmap(ids);

    v(sem);

    if (found_global_id < 0) {
        log_error("No global_id found for local_id %d.", local_id);
        return -1;
    } else {
        log_debug("Global id %d found for local id %d", found_global_id, local_id);
        return found_global_id;
    }

}


int get_local_id(int broker_ids, int global_id) {
    log_debug("Attempting to get local id for global id %d.", global_id);
    int sem = getsem(BROKER_IDS_SEM);
    if (sem < 0) {
        log_error("Error getting broker ids sem.");
        return -1;
    }
    p(sem);

    broker_ids_t* ids = shm_map(broker_ids);

    int found_local_id = -1;
    for (int i = 0; i < ids->count; i++) {
        broker_id_t id = ids->ids[i];
        if (id.global_id == global_id) {
            found_local_id = id.local_id;
            break;
        }
    }

    shm_unmap(ids);

    v(sem);

    if (found_local_id < 0) {
        log_error("No local_id found for global_id %d.", global_id);
        return -1;
    } else {
        log_debug("Local id %d found for global id %d", found_local_id, global_id);
        return found_local_id;
    }
}


bool local_id_exists(int broker_ids, int local_id) {
    log_debug("Checking if local id %d exists.", local_id);
    int sem = getsem(BROKER_IDS_SEM);
    if (sem < 0) {
        log_error("Error getting broker ids sem.");
        return -1;
    }
    p(sem);

    broker_ids_t* ids = shm_map(broker_ids);

    int found_local_id = -1;
    for (int i = 0; i < ids->count; i++) {
        broker_id_t id = ids->ids[i];
        if (id.local_id == local_id) {
            found_local_id = id.local_id;
            break;
        }
    }

    shm_unmap(ids);

    v(sem);

    if (found_local_id < 0) {
        log_info("Local id %d not found.", local_id);
        return false;
    } else {
        log_info("Local id %d found.", local_id);
        return true;
    }
}

int remove_id(int broker_ids, int local_id) {
    log_debug("Attempting to remove local id %d.", local_id);
    int sem = getsem(BROKER_IDS_SEM);
    if (sem < 0) {
        log_error("Error getting broker ids sem.");
        return -1;
    }
    p(sem);

    broker_ids_t* ids = shm_map(broker_ids);

    int pos = -1;
    for (int i = 0; i < ids->count; i++) {
        broker_id_t id = ids->ids[i];
        if (id.local_id == local_id) {
            pos = i;
            break;
        }
    }
    if (pos < 0) {
        log_error("No id found for local_id %d.", local_id);
    } else {
        for (int i = pos + 1; i < ids->count; i++) {
            ids->ids[i - 1] = ids->ids[i];
        }
        ids->count = ids->count - 1;
    }

    shm_unmap(ids);

    v(sem);

    return pos;
}

int broker_id_destroy(int id) {
    log_debug("Deleting broker ids.");
    int res = 0;
    int sem = getsem(BROKER_IDS_SEM);
    if (delsem(sem) < 0) {
        log_error("Error deleting broker ids sem");
        res = -1;
    }
    if (delshm(id) < 0) {
        log_error("Error deleting broker ids shm");
        res = -1;
    }
    return res;
}
