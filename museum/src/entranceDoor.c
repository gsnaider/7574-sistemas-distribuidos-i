#include <stdio.h>
#include <unistd.h>
#include "message.h"
#include "constants.h"
#include "include/msg.h"
#include "include/shm.h"
#include "include/semaphore.h"
#include "include/logger.h"
#include "include/signalUtil.h"


const int PROCESS_ENTRACE_TIME = 2;
bool graceful_quit = false;

void SIGINT_handler(int signum) {
	if (signum != SIGINT) {
		safelog("ENTRANCE DOOR:WARNING: Unkown signal received: %d.", signum);
	} else {
		safelog("ENTRANCE DOOR:SIGINT received, aborting.");
		graceful_quit = true;
	}
}

int main(int argc, char* argv[]) {
	safelog("ENTRANCE DOOR: created.");
	register_handler(SIGINT_handler);

	int req_queue_id;
	sscanf(argv[1], "%d", &req_queue_id);
	int req_queue = getmsg(req_queue_id);

	int resp_queue_id;
	sscanf(argv[2], "%d", &resp_queue_id);
	int resp_queue = getmsg(resp_queue_id);

	if (req_queue < 0) {
		safeperror("ENTRANCE DOOR: Error getting req message queue.");
		exit(-1);
	}
	if (resp_queue < 0) {
		safeperror("ENTRANCE DOOR: Error getting resp message queue.");
		exit(-1);
	}

	int cap_shm_id = getshm(MUSEUM_CAP_SHM);
	if (cap_shm_id < 0) {
		safeperror("ENTRANCE DOOR: ERROR getting cap shared memory");
		exit(-1);
	}
	int* cap_shm = (int*) map(cap_shm_id);

	int cap_sem = getsem(MUSEUM_CAP_SEM);
	if (cap_sem < 0) {
		safeperror("ENTRANCE DOOR: ERROR getting cap semaphore");
		exit(-1);
	}

	int open_shm_id = getshm(MUSEUM_OPEN_SHM);
	if (open_shm_id < 0) {
		safeperror("ENTRANCE DOOR: ERROR getting open shared memory");
		exit(-1);
	}
	bool* open_shm = (bool*) map(open_shm_id);

	while (!graceful_quit) {
		message_t msg;
		rcvmsg(req_queue, &msg, sizeof(message_t), 0);
		if (graceful_quit) {
			break;
		}
		if (msg.type != ENTRANCE_REQUEST) {
			safelog("ENTRANCE DOOR: WARNING: Invalid msg type (%d) received. Discarding msg.", msg.type);
		} else {
			safelog("ENTRANCE DOOR: Processing entrance request from %d", msg.mtype);
			sleep(PROCESS_ENTRACE_TIME);
				p(cap_sem);
				if (*cap_shm > 0 && *open_shm) {
					*cap_shm -= 1;
					msg.type = ACCEPT;
					safelog("ENTRANCE DOOR: Visitor %d accepted", msg.mtype);
				} else {
					msg.type = REJECT;
					safelog("ENTRANCE DOOR: Visitor %d rejected", msg.mtype);
				}
				safelog("ENTRANCE DOOR: Current museum capacity: %d", *cap_shm);
				v(cap_sem);
			sendmsg(resp_queue, &msg, sizeof(message_t));
		}
	}

	if (unmap(cap_shm) < 0) {
		safeperror("ENTRANCE DOOR: Error unmapping cap_shm");
	}

	if (unmap(open_shm) < 0) {
		safeperror("ENTRANCE DOOR: Error unmapping open_shm");
	}

	safelog("ENTRANCE DOOR: Entrance door stopped.");


}