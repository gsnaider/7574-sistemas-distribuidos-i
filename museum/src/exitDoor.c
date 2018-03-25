#include <stdio.h>
#include <unistd.h>

int main(int argc, char* argv[]) {
	printf("Exit door created with pid %d\n", getpid());
	printf("Hi, I'm an exit door with queue ids %s and %s!\n", argv[1], argv[2]);
}