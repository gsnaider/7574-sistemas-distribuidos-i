#ifndef _SEMAFORO_H_
#define _SEMAFORO_H_

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
	return (semget(clave, 1, IPC_CREAT | IPC_EXCL | 0660));
	/* da error si ya existe */
}

/* adquirir derecho de acceso al set de semaforos existentes
 */
int getsem(int identif) {
	key_t clave;
	clave = ftok(DIRECTORY, identif);
	return (semget(clave, 1, 0660));
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
	return (semctl(semid, 0, SETVAL, arg));
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
int elisem(int semid) {
	return (semctl(semid, 0, IPC_RMID, (struct semid_ds *) 0));
}

#endif /* _SEMAFORO_H_ */
