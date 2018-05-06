#include <stdlib.h>
#include "broker_id.h"
#include "../common/log/log.h"
#include "../common/ipc/shm.h"
#include "broker_handler.h"

int broker_id_create() {
    int ids_shm = creashm(BROKER_IDS_SHM, sizeof(broker_id_t) * MAX_CLIENTS);
    if (ids_shm < 0) {
        log_error("Error creating ids shm.");
        exit(-1);
    }
    return ids_shm;
}

int broker_id_get() {
    int ids_shm = getshm(BROKER_IDS_SHM);
    if (ids_shm < 0) {
        log_error("Error getting ids shm.");
        exit(-1);
    }
    return ids_shm;
}

// Adds the local_id to the broker_ids, without a specifying a global_id yet.
int add_local_id(int broker_ids, int local_id) {
    //TODO
    log_debug("Local id %d added to broker ids", local_id);
}

// searches in broker_ids for any local_id without global_id, and assigns the global id to it.
// Returns the local_id to which global_id was assigned.
int add_global_id(int broker_ids, int global_id) {
    int local_id = 0;
    //TODO
    log_debug("Global id %d set to local id %d.", global_id, local_id);
}


int get_global_id(int broker_ids, int local_id) {
    int global_id = 0;
    // TODO
    log_debug("Global id %d found for local id %d", global_id, local_id);
    return 0;
}


int get_local_id(int broker_ids, int global_id) {
    int local_id = 0;
    //TODO
    log_debug("Local id %d found from global_id %d", local_id, global_id);
    return local_id;
}


bool local_id_exists(int broker_ids, int local_id) {
    //TODO
    return true;
}

int broker_id_destroy(int id) {
    log_debug("Deleting broker ids.");
    if (delshm(id) < 0) {
        log_error("Error deleting ids shm");
        return -1;
    }
    return 0;
}
