#include <stdio.h>
#include "constants.h"
#include "include/shm.h"
#include "include/logger.h"

int main(int argc, char* argv[]) {
	safelog("ADMIN: Started");


	int open_shm_id = getshm(MUSEUM_OPEN_SHM);
	if (open_shm_id < 0) {
		safeperror("ADMIN: ERROR getting open shared memory");
		exit(-1);
	}
	bool* open_shm = (bool*) map(open_shm_id);

	int option;
	while (option != 3) {
		printf("Chose an option:\n");
		printf("1. Open museum\n");
		printf("2. Close museum\n");
		printf("3. Exit\n");

		char* option_str;
		scanf("%s", option_str);
		sscanf(option_str, "%d", &option);

		if (option == 1) {
			*open_shm = true;
			safelog("ADMIN: Museum opened");
		} else if (option == 2) {
			*open_shm = false;
			safelog("ADMIN: Museum closed");
		} else if (option != 3) {
			printf("Invalid option!\n");
		}

	}


	if (unmap(open_shm) < 0) {
		safeperror("ADMIN: Error unmapping open_shm");
	}

	safelog("ADMIN: Stopped");
}