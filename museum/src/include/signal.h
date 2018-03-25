#include <signal.h>

void register_handler(void (*handler)(int) ) {
	struct sigaction sa;
	sigemptyset( &sa.sa_mask);
	sigaddset(&sa.sa_mask, SIGINT);
	sa.sa_handler = handler;
	sigaction(SIGINT, &sa, 0);
}