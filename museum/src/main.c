#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include "constants.h"
#include "message.h"
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

void create_door(int req_queue_id, int resp_queue_id, char* exec) {
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
	}
}

void test_msg_queues() {
	int req_queue = getmsg(0);
	int resp_queue = getmsg(1);

	for(int i = 1; i < 15; i++){
		message_t msg = {i, REQUEST};
		printf("Sending message %d\n", i);
		sendmsg(req_queue, &msg, sizeof(message_t));
	
		rcvmsg(resp_queue, &msg, sizeof(message_t), 0);
		printf("Received message %d with type %d\n", i, msg.type);
	}
}

int main(int argc, char* argv[]) {
	printf("Starting museum simulation.\n");

	printf("Creating museum.\n");
	// init shm with museum cap.
	printf("Finished creating museum.\n");


	printf("Starting creation of entrance doors.\n");
	for (int i = 0; i < ENTRANCE_DOORS; i++) {
		create_door(2 * i, 2 * i + 1, "./entranceDoor");
	}
	printf("Finished creation of entrance doors.\n");

	printf("Creating exit door.\n");
	create_door(2 * ENTRANCE_DOORS, 2 * ENTRANCE_DOORS + 1, "./exitDoor");
	printf("Finished creating exit door.\n");

	test_msg_queues();

	// Wait for all processes to finish.
	for (int i = 0; i < ENTRANCE_DOORS + 1; i++) {
		wait((int*) NULL);
	}

	destroy_ipcs();
	exit(0);
}