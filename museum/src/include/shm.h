#ifndef _SHM_H_
#define _SHM_H_

#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#include "logger.h"
#include "resources.h"

int   getshm(int);
int   creashm(int,int);
void* map(int);
int   unmap(void*);
int   elishm(int);

int   creashm(int id, int size){
    key_t clave;
    clave = ftok(DIRECTORY, id);
    if (clave < 0) {
        safeperror("Error on ftok while creating shared memory");
        return -1;
    }
    /* da error si ya existe */
    int shm_id = shmget(clave, size, IPC_CREAT | IPC_EXCL | 0660);
    if (shm_id < 0) {
        safeperror("Error on shmget while creating shared memory");
    }
    return shm_id;
}

int   getshm(int id){
    key_t clave;
    clave = ftok(DIRECTORY, id);
    if (clave < 0) {
        safeperror("Error on ftok while getting shared memory");
        return -1;
    }
    int shm_id = shmget(clave, 1, 0660);
    if (shm_id < 0) {
        safeperror("Error on shmget while getting shared memory");
    }
    return shm_id;
}

void* map(int id){
    void*addr = shmat(id, NULL, 0);
    if(addr==(void*)-1){
        safeperror("No se pudo mapear la memoria");
        exit(-1);
    }
    return addr;
}

int   unmap(void* addr){
    int res = shmdt(addr);
    if (res < 0) {
        safeperror("Error unmapping shared memory");
    }
    return res;
}

int   elishm(int id){
    int res = shmctl(id, IPC_RMID, (struct shmid_ds *) 0);
    if (res < 0) {
        safeperror("Error deleting shared memory");
    }
    return res;   
}

#endif /* _SHM_H_ */
