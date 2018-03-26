#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include "constants.h"
#include "message.h"
#include "include/msg.h"
#include "include/shm.h"
#include "include/semaphore.h"
#include "include/logger.h"
#include "include/signalUtil.h"

// Entrance doors + exit door + person generator.
const int MAX_CHILDS = ENTRANCE_DOORS + 2;
pid_t childs_arr[MAX_CHILDS];
int childs = 0;

void delete_msg_queue(int id) {
	safelog("Deleting message queue %d", id);
	int queue = getmsg(id);
	if (queue < 0) {
		safeperror("Error getting message queue %d for deletion.", id);
	} else {
		if (delmsg(queue) < 0) {
			safeperror("Error deleting message queue %d.", id);
		}
	}
}

void delete_shm(int id) {
	safelog("Deleting shared memory %d", id);
	int shm= getshm(id);
	if (shm < 0) {
		safeperror("ERROR getting shm %d for deletion", id);
	} else {
		if (delshm(shm) < 0) {
			safeperror("Error deleting shm %d.", id);
		}
	}
}

void delete_sem(int id) {
	safelog("Deleting semaphore %d", id);
	int sem = getsem(id);
	if (sem < 0) {
		safeperror("ERROR getting sem %d for deletion", id);
	} else {
		if (delsem(sem) < 0) {
			safeperror("Error deleting sem %d.", id);
		}
	}
}

void destroy_ipcs() {
	safelog("Destrying ipcs");
	for (int i = 0; i < ENTRANCE_DOORS; i++) {
		delete_msg_queue(2 * i);
		delete_msg_queue(2 * i + 1);
	}
	delete_msg_queue(2 * ENTRANCE_DOORS);
	delete_msg_queue(2 * ENTRANCE_DOORS + 1);

	delete_sem(MUSEUM_CAP_SEM);
	delete_shm(MUSEUM_CAP_SHM);

}

void wait_childs() {
	for (int i = 0; i < childs; i++) {
		wait((int*) NULL);
	}
}

void kill_childs() {
	for (int i = 0; i < childs; i++) {
		safelog("Killing child %d", childs_arr[i]);
		if (kill(childs_arr[i], SIGINT) < 0) {
			safeperror("Error killing child %d", childs_arr[i]);
		}
	}
}

void safe_exit() {
	kill_childs();
	wait_childs();
	destroy_ipcs();
	exit(-1);
}

void safe_exit(const char* exit_msg) {
	safeperror("%s", exit_msg);
	safe_exit();
}

void SIGINT_handler(int signum) {
	if (signum != SIGINT) {
		safelog("WARNING: Unkown signal received: %d.", signum);
	} else {
		safelog("SIGINT received, aborting.");
		safe_exit();
	}
}

void create_door(int req_queue_id, int resp_queue_id, const char* exec) {
	if (creamsg(req_queue_id) < 0) {
		safe_exit("ERROR creating req msg queue.");
	}
	if (creamsg(resp_queue_id) < 0) {
		safe_exit("ERROR creating resp msg queue.");
	}

	char req_queue_str[3];
	snprintf(req_queue_str, 3, "%d", req_queue_id);

	char resp_queue_str[3];
	snprintf(resp_queue_str, 3, "%d", resp_queue_id);

	pid_t pid = fork();
	if (pid < 0) {
		safe_exit("ERROR forking door");
	}
	if (pid == 0) {
		execl(exec, exec, req_queue_str, resp_queue_str, (char*)NULL);
		safe_exit("ERROR executing door");
	} else {
		childs_arr[childs++] = pid;
	}
}

void create_museum() {
	safelog("Creating museum.");
	int sem = creasem(MUSEUM_CAP_SEM);
	if (sem < 0) {
		safe_exit("ERROR creating semaphore");
	}
	inisem(sem, 1);

	int shm_id = creashm(MUSEUM_CAP_SHM, sizeof(int));
	if (shm_id < 0) {
		safe_exit("ERROR creating shared memory");
	}
	int* shm = (int*) map(shm_id);

	*shm = MUSEUM_CAP;

	safelog("Finished creating museum.");
}

int main(int argc, char* argv[]) {
	safelog("Starting museum simulation.");
	register_handler(SIGINT_handler);

	create_museum();

	safelog("Starting creation of entrance doors.");
	for (int i = 0; i < ENTRANCE_DOORS; i++) {
		create_door(2 * i, 2 * i + 1, "./entranceDoor");
	}
	safelog("Finished creation of entrance doors.");

	safelog("Creating exit door.");
	create_door(2 * ENTRANCE_DOORS, 2 * ENTRANCE_DOORS + 1, "./exitDoor");
	safelog("Finished creating exit door.");

	safelog("Starting person generator.");
	pid_t person_gen_pid = fork();
	if (person_gen_pid < 0) {
		safe_exit("ERROR forking person generator");
	}
	if (person_gen_pid == 0) {
		execl("./personGen", "./personGen", (char*)NULL);
		safe_exit("ERROR executing person generator");
	} else {
		childs_arr[childs++] = person_gen_pid;
	}

	wait_childs();
	destroy_ipcs();
	exit(0);
}