#include <stdio.h>
#include <unistd.h>
#include "message.h"
#include "constants.h"
#include "include/msg.h"
#include "include/shm.h"
#include "include/semaphore.h"
#include "include/logger.h"
#include "include/signalUtil.h"

bool graceful_quit = false;

void SIGINT_handler(int signum) {
	if (signum != SIGINT) {
		safelog("EXIT DOOR: WARNING: Unkown signal received: %d.", signum);
	} else {
		safelog("EXIT DOOR: SIGINT received, aborting.");
		graceful_quit = true;
	}
}

int main(int argc, char* argv[]) {
	safelog("EXIT DOOR: started.");
	register_handler(SIGINT_handler);

	int req_queue_id;
	sscanf(argv[1], "%d", &req_queue_id);
	int req_queue = getmsg(req_queue_id);

	int resp_queue_id;
	sscanf(argv[2], "%d", &resp_queue_id);
	int resp_queue = getmsg(resp_queue_id);

	if (req_queue < 0) {
		safeperror("EXIT DOOR: Error getting req message queue.");
		exit(-1);
	}
	if (resp_queue < 0) {
		safeperror("EXIT DOOR: Error getting resp message queue.");
		exit(-1);
	}

	int shm_id = getshm(MUSEUM_CAP_SHM);
	if (shm_id < 0) {
		safeperror("EXIT DOOR: ERROR getting shared memory");
		exit(-1);
	}
	int* shm = (int*) map(shm_id);

	int sem = getsem(MUSEUM_CAP_SEM);
	if (sem < 0) {
		safeperror("EXIT DOOR: ERROR getting semaphore");
		exit(-1);
	}

	while (!graceful_quit) {
		message_t msg;
		rcvmsg(req_queue, &msg, sizeof(message_t), 0);
		if (graceful_quit) {
			break;
		}
		if (msg.type != EXIT_REQUEST) {
			safelog("EXIT DOOR: WARNING: Invalid msg type (%d) received. Discarding msg.", msg.type);
		} else {
			p(sem);
			*shm += 1;
			msg.type = ACCEPT;
			safelog("EXIT DOOR: Visitor %d allowed to exit", msg.mtype);
			safelog("EXIT DOOR: Current museum capacity: %d", *shm);
			v(sem);
			sendmsg(resp_queue, &msg, sizeof(message_t));
		}
	}

	if (unmap(shm) < 0) {
		safeperror("EXIT DOOR: Error unmapping shm");
	}

	safelog("EXIT DOOR: stopped.");



}