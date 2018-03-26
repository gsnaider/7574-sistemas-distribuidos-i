#include <stdio.h>
#include "include/logger.h"


void open_museum() {
	safelog("Opening museum\n");
}

void close_museum() {
	safelog("Closing museum\n");
}

int main(int argc, char* argv[]) {
	safelog("ADMIN: Started");

	int option;

	while(option != 3) {
		printf("Chose an option:\n");
		printf("1. Open museum\n");
		printf("2. Close museum\n");	
		printf("3. Exit\n");	
		scanf("%d", &option);
		if (option == 1) {
			open_museum();
		} else if (option == 2) {
			close_museum();
		} else if (option != 3) {
			printf("Invalid option!\n");	
		}

	}

	safelog("ADMIN: Stopped");
}