#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "constants.h"
#include "include/msg.h"


void destroy_ipcs() {
	printf("Destrying ipcs\n");
	if (fork() == 0) {
		execl("./ipcrm.sh", "./ipcrm.sh", (char*)NULL);
		printf("ERROR destrying ipcs.\n");
	}
}

void safe_exit(char* exit_msg) {
	printf("%s\n", exit_msg);
	destroy_ipcs();
	exit(-1);
}

int main(int argc, char* argv[]) {
	printf("Starting museum simulation.\n");

	printf("Starting creation of entrance doors.\n");
	for (int i = 0; i < ENTRANCE_DOORS; i++) {
		int entrace_req_msg_queue_id = 2 * i;
		int entrace_resp_msg_queue_id = 2 * i + 1;

		if (creamsg(entrace_req_msg_queue_id) < 0) {
			safe_exit("ERROR creating entrance req msg queue.");
		}
		if (creamsg(entrace_resp_msg_queue_id) < 0) {
			safe_exit("ERROR creating entrance resp msg queue.");
		}

		char req_queue_str[3];
		snprintf(req_queue_str, 3, "%d", entrace_req_msg_queue_id);

		char resp_queue_str[3];
		snprintf(resp_queue_str, 3, "%d", entrace_resp_msg_queue_id);

		pid_t pid = fork();
		if (pid < 0) {
			safe_exit("ERROR forking entrance door");
		}
		if (pid == 0) {
			execl("./entranceDoor", "./entranceDoor", req_queue_str, resp_queue_str, (char*)NULL);
			safe_exit("ERROR executing entrance door");
		}
	}

	// Wait for all processes to finish.
	for (int i = 0; i < ENTRANCE_DOORS; i++) {
		wait((int*) NULL);
	}

	destroy_ipcs();
	exit(0);
}