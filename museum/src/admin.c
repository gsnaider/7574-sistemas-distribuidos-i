#include <stdio.h>
#include "include/logger.h"


void open_museum() {
	safelog("Opening museum");
}

void close_museum() {
	safelog("Closing museum");
}

int main(int argc, char* argv[]) {
	safelog("ADMIN: Started");

	int option;

	while(option != 3) {
		printf("Chose an option:\n");
		printf("1. Open museum\n");
		printf("2. Close museum\n");	
		printf("3. Exit\n");
		
		char* option_str;
		scanf("%s", option_str);
		sscanf(option_str, "%d", &option);

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