#include "semaphore.h"

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
// #include "log.h"
#include <assert.h>

// Constant to pass as second parameter for keys
#define MAGIC_NUM 6
#define SEM_PERM 0666

#define SEM_OP_FLAG 0 // Or SEM_UNDO


/*
 * Gets the semaphore set associated with file filename.
 * If the semaphore set does not exist, it will be created.
 *
 * Returns semid on success, negative on error.
 */
int sem_get(char* filename, int semnum) {
	assert(semnum >= 0);
	key_t key = ftok(filename, MAGIC_NUM);
	if(key < 0) {
		// log_write(ERROR_L, "sem_get: ftok failure\n");
		return -1;
	}
	return semget(key, semnum, IPC_CREAT | SEM_PERM);
}

/*
 * Sets the initial value of the semnum-th semaphore of set
 * with semid to value.
 *
 * Returns 0 on success, negative on error.
 */
int sem_init(int semid, int semnum, int value) {
	assert(value >= 0);
	assert(semnum >= 0);
	return semctl(semid, semnum, SETVAL, value);
}

/*
 * Set the initial value of the first semnum semaphores of set
 * with semid to value.
 *
 * Returns 0 on success, negative on error.
 * If an errors ocurs while semctl-ing one of the semaphores,
 * operation is halted and negative value is returned.
 */
int sem_init_all(int semid, int semnum, int value) {
	int i, r;
	for (i = 0; i < semnum; i++)
		if ((r = sem_init(semid, i, value)) < 0)
			return r;
	return 0;
}


/*
 * Generic v() operation on the semnum-th semaphore of
 * set semid. It will atomically increase the value of
 * the semaphore by value in a non-blocking fashion.
 */
int sem_put(int semid, unsigned int semnum, unsigned int value) {
	assert(value > 0);
	assert(semnum >= 0);
	struct sembuf sop = {semnum, value, SEM_OP_FLAG};
	return semop(semid, &sop, 1);
}

/*
 * Generic p() operation on the semnum-th semaphore of
 * set semid. It will try to decrement semaphore's value
 * by value, blocking if that will induce a negative number.
 */
int sem_take(int semid, unsigned int semnum, unsigned int value) {
	assert(value > 0);
	assert(semnum >= 0);
	struct sembuf sop = {semnum, -value, SEM_OP_FLAG};
	return semop(semid, &sop, 1);
}

/*
 * Try to decrement by one semnum-th semaphore of set semid.
 * Block if it induces a negative number and only wakes when
 * someone makes a sem_post() on the semaphore.
 *
 * Also a signal can awaik a sleeping process.
 */
int sem_wait(int semid, unsigned int semnum) {
	return sem_take(semid, semnum, 1);
}

/*
 * Increment by one the semnum-th semaphore of set semid, and
 * wakens any other process that were blocked on a sem_wait()
 * on the same semaphore.
 */
int sem_post(int semid, unsigned int semnum) {
	return sem_put(semid, semnum, 1);	
}

/*
 * Performs a wait on semnum-th semaphore of set semid until
 * its value becomes 0, it is destroyed or a signal arrives.
 */
int sem_waitz(int semid, unsigned int semnum) {
	assert(semnum >= 0);
	struct sembuf sop = {semnum, 0, SEM_OP_FLAG};
	return semop(semid, &sop, 1);
}

/*
 * Destroys semaphore set with semid and frees its resources.
 */
int sem_destroy(int semid) {
	// second parameter (semnum) is ignored.
	return semctl(semid, 0, IPC_RMID); 
}
