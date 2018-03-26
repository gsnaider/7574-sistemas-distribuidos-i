#ifndef _MUSEUM_API_H_
#define _MUSEUM_API_H_

#include "include/logger.h"
#include "include/msg.h"
#include "include/shm.h"
#include "include/semaphore.h"
#include "constants.h"
#include "message.h"

const int TOUR_TIME = 10;

void go_on_tour() {

	int req_queue = getmsg(TOUR_REQ_MSG);
	if (req_queue < 0) {
		safeperror("PERSON ON TOUR: Error getting req message queue.");
		exit(-1);
	}
	int resp_queue = getmsg(TOUR_RESP_MSG);
	if (resp_queue < 0) {
		safeperror("PERSON ON TOUR: Error getting resp message queue.");
		exit(-1);
	}

	int tour_count_shm_id = getshm(MUSEUM_TOUR_COUNT_SHM);
	if (tour_count_shm_id < 0) {
		safeperror("PERSON ON TOUR: ERROR getting tour count shared memory");
		exit(-1);
	}
	int* tour_count_shm = (int*) map(tour_count_shm_id);

	int tour_count_sem = getsem(MUSEUM_TOUR_COUNT_SEM);
	if (tour_count_sem < 0) {
		safeperror("PERSON ON TOUR: ERROR getting tour count semaphore");
		exit(-1);
	}


	int tour_start_sem = getsem(MUSEUM_TOUR_START_SEM);
	if (tour_start_sem < 0) {
		safeperror("PERSON ON TOUR: ERROR getting tour start semaphore");
		exit(-1);
	}

	p(tour_count_sem);
	if (*tour_count_shm < TOUR_CAP) {
		safelog("PERSON ON TOUR: Registering on tour");
		*tour_count_shm += 1;
		safelog("PERSON ON TOUR: People waiting on tour: %d", *tour_count_shm);
		
		message_t msg = {getpid(), TOUR_REQUEST};
		sendmsg(req_queue, &msg, sizeof(message_t));

		if (*tour_count_shm == TOUR_CAP) {
			v(tour_start_sem);
		}

		v(tour_count_sem);
		rcvmsg(resp_queue, &msg, sizeof(message_t), getpid());
		if (msg.type == TOUR_START) {
			safelog("PERSON ON TOUR: On tour");
			sleep(TOUR_TIME);
		} else {
			safelog("PERSON ON TOUR: WARNING: Invalid msg type (%d) received. Returning to person.", msg.type);
		}
	} else {
		v(tour_count_sem);
	}


	if (unmap(tour_count_shm) < 0) {
		safeperror("EXIT DOOR: Error unmapping tour_count_shm");
	}
}

#endif
