#include <stdio.h>
#include "include/logger.h"
#include "include/signalUtil.h"

const int TIME_BETWEEN_PERSONS = 1;
bool graceful_quit = false;

void SIGINT_handler(int signum) {
	if (signum != SIGINT) {
		safelog("WARNING: Unkown signal received: %d.", signum);
	} else {
		safelog("SIGINT received, aborting.");
		graceful_quit = true;
	}
}

int main(int argc, char* argv[]) {
	safelog("Starting person generator");
	register_handler(SIGINT_handler);

	while (!graceful_quit) {
		sleep(TIME_BETWEEN_PERSONS);
		pid_t pid = fork();
		if (pid < 0) {
			safeperror("ERROR forking person");
			exit(-1);
		}
		if (pid == 0) {
			execl("./person", "./person", (char*)NULL);
			safeperror("ERROR executing person");
			exit(-1);
		}
	}

	safelog("Person generator stopped");
}