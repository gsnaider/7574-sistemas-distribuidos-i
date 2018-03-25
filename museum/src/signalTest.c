#include <signal.h>
#include <unistd.h>
#include <stdio.h>
#include <assert.h>


bool quit = false;

void SIGINT_handler(int signum) {
	assert(signum == SIGINT);
	quit = true;
}

void register_handler() {
	struct sigaction sa;
	sigemptyset( &sa.sa_mask);
	sigaddset(&sa.sa_mask, SIGINT);
	sa.sa_handler = SIGINT_handler;
	sigaction(SIGINT, &sa, 0);
}

int main(int argc, char* argv[]) {
	register_handler();
	while (!quit) {
		printf("Soy el proceso %d\n", getpid());
		sleep(2);
	}

	printf("Fin del proceso.\n");
	return 0;
}