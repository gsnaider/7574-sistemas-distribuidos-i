#include <stdio.h>
#include <unistd.h>
#include "message.h"
#include "include/msg.h"


int main(int argc, char* argv[]) {
	printf("Entrance door created with pid %d\n", getpid());
	
	int req_queue_id;
    sscanf(argv[1], "%d", &req_queue_id);
    int req_queue = getmsg(req_queue_id);

	int resp_queue_id;
    sscanf(argv[2], "%d", &resp_queue_id);
    int resp_queue = getmsg(resp_queue_id);

	if (req_queue < 0) {
		printf("Error getting req message queue.\n");
		exit(-1);
	}
	if (resp_queue < 0) {
		printf("Error getting resp message queue.\n");
		exit(-1);
	}



	while (true) {
		message_t msg;
		rcvmsg(req_queue, &msg, sizeof(message_t), 0);
		printf("Processing request...\n");
		sleep(1);

		//TODO: Check shm if theres space.

		msg.type = ACCEPT;
		printf("Finished processing request\n");
		sendmsg(resp_queue, &msg, sizeof(message_t));
	}

}