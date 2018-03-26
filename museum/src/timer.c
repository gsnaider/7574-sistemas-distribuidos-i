#include "constants.h"
#include "include/logger.h"
#include "include/shm.h"
#include "include/semaphore.h"
#include "include/signalUtil.h"

bool graceful_quit = false;

void SIGINT_handler(int signum) {
	if (signum != SIGINT) {
		safelog("TIMER: WARNING: Unkown signal received: %d.", signum);
	} else {
		safelog("TIMER: SIGINT received, aborting.");
		graceful_quit = true;
	}
}

int main(int argc, char* argv[]) {
	safelog("TIMER: timer started");
	register_handler(SIGINT_handler);

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
		sleep(TOUR_WAIT_TIME);
		if (graceful_quit) {
			break;
		}
		safelog("TIMER: TOUR TIME!");
		p(tour_count_sem);
		if (*tour_count_shm > 0) {
			v(tour_start_sem);
		} else {
			safelog("TIMER: No people for tour.");
		}

		v(tour_count_sem);
	}

	if (unmap(tour_count_shm) < 0) {
		safeperror("TIMER: Error unmapping tour_count_shm");
	}

	safelog("TIMER: timer stopped");



}
