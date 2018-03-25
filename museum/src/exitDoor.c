#include <stdio.h>
#include <unistd.h>
#include "message.h"
#include "constants.h"
#include "include/msg.h"
#include "include/shm.h"
#include "include/semaphore.h"
#include "include/logger.h"

bool graceful_quit = false;

int main(int argc, char* argv[]) {
	safelog("Exit door created.");

	int req_queue_id;
	sscanf(argv[1], "%d", &req_queue_id);
	int req_queue = getmsg(req_queue_id);

	int resp_queue_id;
	sscanf(argv[2], "%d", &resp_queue_id);
	int resp_queue = getmsg(resp_queue_id);

	if (req_queue < 0) {
		safeperror("Error getting req message queue.");
		exit(-1);
	}
	if (resp_queue < 0) {
		safeperror("Error getting resp message queue.");
		exit(-1);
	}

	int shm_id = getshm(MUSEUM_CAP_SHM);
	if (shm_id < 0) {
		safeperror("ERROR getting shared memory");
		exit(-1);
	}
	int* shm = (int*) map(shm_id);

	int sem = getsem(MUSEUM_CAP_SEM);
	if (sem < 0) {
		safeperror("ERROR getting semaphore");
		exit(-1);
	}

	while (!graceful_quit) {
		message_t msg;
		safelog("Waiting requests");
		rcvmsg(req_queue, &msg, sizeof(message_t), 0);
		if (msg.type != EXIT_REQUEST) {
			safelog("WARNING: Invalid msg type (%d) received on exit door. Discarding msg.", msg.type);
		} else {
			safelog("Processing request...");
			sleep(5);
			p(sem);
			*shm += 1;
			msg.type = ACCEPT;
			safelog("Visitor %d allowed to exit", msg.mtype);
			safelog("Current museum capacity: %d", *shm);
			v(sem);
			safelog("Finished processing request");
			safelog("Sending response");
			sendmsg(resp_queue, &msg, sizeof(message_t));
		}
	}

}