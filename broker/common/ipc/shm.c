//
// Created by gaston on 22/04/18.
//

#include <sys/ipc.h>
#include <stdlib.h>
#include <sys/shm.h>
#include "shm.h"
#include "resources.h"
#include "../log/log.h"

int   creashm(int id, int size){
    key_t clave;
    clave = ftok(DIRECTORY, id);
    if (clave < 0) {
        log_error("Error on ftok while creating shared memory");
        return -1;
    }
    /* da error si ya existe */
    int shm_id = shmget(clave, size, IPC_CREAT | IPC_EXCL | 0660);
    if (shm_id < 0) {
        log_error("Error on shmget while creating shared memory");
    }
    return shm_id;
}

int   getshm(int id){
    key_t clave;
    clave = ftok(DIRECTORY, id);
    if (clave < 0) {
        log_error("Error on ftok while getting shared memory");
        return -1;
    }
    int shm_id = shmget(clave, 1, 0660);
    if (shm_id < 0) {
        log_error("Error on shmget while getting shared memory");
    }
    return shm_id;
}

void* shm_map(int id){
    void*addr = shmat(id, NULL, 0);
    if(addr==(void*)-1){
        log_error("No se pudo mapear la memoria");
        exit(-1);
    }
    return addr;
}

int   shm_unmap(void *addr){
    int res = shmdt(addr);
    if (res < 0) {
        log_error("Error unmapping shared memory");
    }
    return res;
}

int   delshm(int id){
    int res = shmctl(id, IPC_RMID, (struct shmid_ds *) 0);
    if (res < 0) {
        log_error("Error deleting shared memory");
    }
    return res;
}