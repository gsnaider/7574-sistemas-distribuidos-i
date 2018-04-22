//
// Created by gaston on 22/04/18.
//

#include <sys/ipc.h>
#include <sys/sem.h>
#include <stdlib.h>
#include "semaphore.h"
#include "resources.h"
#include "../log/log.h"


int creasem(int identif) {
    key_t clave;
    clave = ftok(DIRECTORY, identif);
    if (clave < 0) {
        log_error("Error on ftok while creating semaphore");
        return -1;
    }
    /* da error si ya existe */
    int sem_id = semget(clave, 1, IPC_CREAT | IPC_EXCL | 0660);
    if (sem_id < 0) {
        log_error("Error on semget while creating semaphore");
    }
    return sem_id;
}

int getsem(int identif) {
    key_t clave;
    clave = ftok(DIRECTORY, identif);
    if (clave < 0) {
        log_error("Error on ftok while getting semaphore");
        return -1;
    }
    int sem_id = semget(clave, 1, 0660);
    if (sem_id < 0) {
        log_error("Error on semget while getting semaphore");
    }
    return sem_id;
}

int inisem(int semid, int val) {
    union semun {
        int val;
        /* Value for SETVAL */
        struct semid_ds *buf;
        /* Buffer for IPC_STAT, IPC_SET */
        unsigned short *array; /* Array for GETALL, SETALL */
        struct seminfo *__buf; /* Buffer for IPC_INFO(Linux specific)*/
    } arg;
    arg.val = val;
    int res = semctl(semid, 0, SETVAL, arg);
    if (res < 0) {
        log_error("Error on semctl while initializing semaphore");
    }
    return res;
}

int p(int semid) {
    struct sembuf oper;
    oper.sem_num = 0;
    /* nro. de semáforo del set */
    oper.sem_op = -1;
    /* p(sem) */
    oper.sem_flg = 0;

    if(semop(semid, &oper, 1)!=0){
        log_error("No se pudo hacer P del semáforo");
        exit(-1);
    }

    return 1;
}

int v(int semid) {
    struct sembuf oper;
    oper.sem_num = 0;
    /* nro. de semáforo */
    oper.sem_op = 1;
    /* v(sem) */
    oper.sem_flg = 0;
    if(semop(semid, &oper, 1)!=0){
        log_error("No se pudo hacer V del semáforo");
        exit(-1);
    }
    return 1;
}

int delsem(int semid) {
    int res = semctl(semid, 0, IPC_RMID, (struct semid_ds *) 0);
    if (res < 0) {
        log_error("Error deleting semaphore");
    }
    return res;
}