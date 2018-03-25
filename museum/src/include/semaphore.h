#ifndef _SEMAPHORE_H_
#define _SEMAPHORE_H_

#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/sem.h>

#include "logger.h"
#include "resources.h"

	
/* Funciones de semaforos crear el set de semaforos (si no existe)
 */
int creasem(int identif) {
	key_t clave;
	clave = ftok(DIRECTORY, identif);
	if (clave < 0) {
		safeperror("Error on ftok while creating semaphore");
		return -1;
	}
	/* da error si ya existe */
	int sem_id = semget(clave, 1, IPC_CREAT | IPC_EXCL | 0660);
	if (sem_id < 0) {
		safeperror("Error on semget while creating semaphore");
	}
	return sem_id;
}

/* adquirir derecho de acceso al set de semaforos existentes
 */
int getsem(int identif) {
	key_t clave;
	clave = ftok(DIRECTORY, identif);
	if (clave < 0) {
		safeperror("Error on ftok while getting semaphore");
		return -1;
	}
	int sem_id = semget(clave, 1, 0660);
	if (sem_id < 0) {
		safeperror("Error on semget while getting semaphore");
	}
	return sem_id;
}

/* inicializar al semáforo del set de semaforos
 */
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
		safeperror("Error on semctl while initializing semaphore");
	}
	return res;
}

/* ocupar al semáforo (p) WAIT
 */
int p(int semid) {
	struct sembuf oper;
	oper.sem_num = 0;
	/* nro. de semáforo del set */
	oper.sem_op = -1;
	/* p(sem) */
	oper.sem_flg = 0;

	if(semop(semid, &oper, 1)!=0){
	  safeperror("No se pudo hacer P del semáforo");
	  exit(-1);
	}

	return 1;
}

/* liberar al semáforo (v) SIGNAL
 */
int v(int semid) {
	struct sembuf oper;
	oper.sem_num = 0;
	/* nro. de semáforo */
	oper.sem_op = 1;
	/* v(sem) */
	oper.sem_flg = 0;
	if(semop(semid, &oper, 1)!=0){
	  safeperror("No se pudo hacer V del semáforo");
	  exit(-1);
	}
	return 1;
}

/* eliminar el set de semaforos
 */
int delsem(int semid) {
	return (semctl(semid, 0, IPC_RMID, (struct semid_ds *) 0));
}

#endif /* _SEMAPHORE_H_ */
