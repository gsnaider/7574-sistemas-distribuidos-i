#include "include/logger.h"
#include "include/msg.h"
#include "include/shm.h"
#include "include/semaphore.h"
#include "include/signalUtil.h"
#include "constants.h"
#include "message.h"


bool graceful_quit = false;
pid_t timer_pid;

void SIGINT_handler(int signum) {
	if (signum != SIGINT) {
		safelog("GUIDE: WARNING: Unkown signal received: %d.", signum);
	} else {
		safelog("GUIDE: SIGINT received, aborting.");
		if (kill(timer_pid, SIGINT) < 0) {
			safeperror("Error killing timer: %d", timer_pid);
		}
		graceful_quit = true;
	}
}

int main(int argc, char* argv[]) {
	safelog("GUIDE: started.");
	register_handler(SIGINT_handler);

	safelog("GUIDE: Starting timer.");
	timer_pid = fork();
	if (timer_pid < 0) {
		safeperror("GUIDE: ERROR forking timer");
		exit(-1);
	}
	if (timer_pid == 0) {
		execl("./timer", "./timer", (char*)NULL);
		safeperror("GUIDE: ERROR executing timer");
		exit(-1);
	}
	safelog("GUIDE: Timer started.");
	

	int req_queue = getmsg(TOUR_REQ_MSG);
	if (req_queue < 0) {
		safeperror("GUIDE: Error getting req message queue.");
		exit(-1);
	}
	int resp_queue = getmsg(TOUR_RESP_MSG);
	if (resp_queue < 0) {
		safeperror("GUIDE: Error getting resp message queue.");
		exit(-1);
	}

	int tour_count_shm_id = getshm(MUSEUM_TOUR_COUNT_SHM);
	if (tour_count_shm_id < 0) {
		safeperror("GUIDE: ERROR getting tour count shared memory");
		exit(-1);
	}
	int* tour_count_shm = (int*) map(tour_count_shm_id);

	int tour_count_sem = getsem(MUSEUM_TOUR_COUNT_SEM);
	if (tour_count_sem < 0) {
		safeperror("GUIDE: ERROR getting tour count semaphore");
		exit(-1);
	}


	int tour_start_sem = getsem(MUSEUM_TOUR_START_SEM);
	if (tour_start_sem < 0) {
		safeperror("GUIDE: ERROR getting tour start semaphore");
		exit(-1);
	}


	while(!graceful_quit) {

		p(tour_start_sem);
		if (graceful_quit) {
			break;
		}
		safelog("GUIDE: Reading people for tour...");
		p(tour_count_sem);
		int people_for_tour = *tour_count_shm;
		for (int i = 0; i < people_for_tour;) {
			message_t msg;
			rcvmsg(req_queue, &msg, sizeof(message_t), 0);
			if (msg.type == TOUR_REQUEST) {
				msg.type = TOUR_START;
				sendmsg(resp_queue, &msg, sizeof(message_t));
			} else {
				safelog("GUIDE: WARNING: Invalid msg type (%d) received. Discarding msg.", msg.type);
			}

			*tour_count_shm -= 1;
		}
		if (graceful_quit) {
			break;
		}

		v(tour_count_sem);

	}


	if (unmap(tour_count_shm) < 0) {
		safeperror("GUIDE: Error unmapping tour_count_shm");
	}

}