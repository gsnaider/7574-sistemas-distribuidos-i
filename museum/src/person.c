#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include "include/msg.h"
#include "include/logger.h"
#include "constants.h"
#include "message.h"

const int MAX_TIME_IN_MUSEUM = 20;

int main(int argc, char* argv[]) {
	srand(time(NULL));
	safelog("PERSON: Arrived");

	int entrance_door = rand() % ENTRANCE_DOORS;
	safelog("PERSON: Chose door %d", entrance_door);

	int entrance_req_queue = getmsg(2 * entrance_door);
	if (entrance_req_queue < 0) {
		safeperror("PERSON: ERROR getting entrance request msg queue.");
		exit(-1);
	}

	int entrance_resp_queue = getmsg(2 * entrance_door + 1);
	if (entrance_resp_queue < 0) {
		safeperror("PERSON: ERROR getting entrance response msg queue.");
		exit(-1);
	}

	message_t msg = {getpid(), ENTRANCE_REQUEST};
	sendmsg(entrance_req_queue, &msg, sizeof(message_t));
	rcvmsg(entrance_resp_queue, &msg, sizeof(message_t), getpid());

	if (msg.type == ACCEPT) {
		safelog("PERSON: Entering museum...");
		sleep(rand() % MAX_TIME_IN_MUSEUM);
		
		int exit_req_queue = getmsg(2 * ENTRANCE_DOORS);
		if (exit_req_queue < 0) {
			safeperror("PERSON: ERROR getting exit request msg queue.");
			exit(-1);
		}

		int exit_resp_queue = getmsg(2 * ENTRANCE_DOORS + 1);
		if (exit_resp_queue < 0) {
			safeperror("PERSON: ERROR getting exit response msg queue.");
			exit(-1);
		}

		msg.type = EXIT_REQUEST;
		safelog("PERSON: Exiting museum");
		sendmsg(exit_req_queue, &msg, sizeof(message_t));
		rcvmsg(exit_resp_queue, &msg, sizeof(message_t), getpid());
		if (msg.type != ACCEPT) {
			safelog("PERSON: WARNING: Exit was rejected. Stopping person");
			exit(-1);
		}
	} else {
		safelog("PERSON: Couldn't enter museum");
	}

	safelog("PERSON: exited");
}
