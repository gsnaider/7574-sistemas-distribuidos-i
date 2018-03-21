#include <stdio.h>
#include <unistd.h> 
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

void create_ipcs() {
	pid_t ipcs_gen_id = fork();
	if (ipcs_gen_id == 0) {
		execl("./ipcsGen", "./ipcsGen", (char*)NULL);
		printf("Error calling ipcsGen\n");
		exit(-1);
	}
	int status;
	waitpid(ipcs_gen_id, &status, 0);
	if (status < 0) {
		printf("Error executing ipcsGen\n");
		exit(-1);
	}
}

int main(int argc, char* argv[]) {
	printf("Starting museum simulation.\n");
	create_ipcs();

}