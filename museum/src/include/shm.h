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
    return (shmget(clave, size, IPC_CREAT | IPC_EXCL | 0660));
    /* da error si ya existe */
}

int   getshm(int id){
    key_t clave;
    clave = ftok(DIRECTORY, id);
    return (shmget(clave, 1, 0660));
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
    return shmdt(addr);
}

int   elishm(int id){
    return (shmctl(id, IPC_RMID, (struct shmid_ds *) 0));
}

#endif /* _SHM_H_ */
