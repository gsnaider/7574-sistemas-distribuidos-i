#ifndef SEMAPHORE_H
#define SEMAPHORE_H
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>

#include <assert.h>

// Creation & initialization
int sem_get(char* filename, int semnum);
int sem_init(int semid, int semnum, int value);
int sem_init_all(int semid, int semnum, int value);

// Operations
int sem_put(int semid, unsigned int semnum, unsigned int value);
int sem_take(int semid, unsigned int semnum, unsigned int value);
int sem_wait(int semid, unsigned int semnum);
int sem_post(int semid, unsigned int semnum);
int sem_waitz(int semid, unsigned int semnum);

// Destructor
int sem_destroy(int semid);

#endif // SEMAPHORE_H
