//
// Created by gaston on 15/05/18.
//

#include <stdbool.h>
#include <unistd.h>
#include <stdlib.h>
#include "../common/log/log.h"

bool quit = false;

static int read_option() {
    char buffer[10];
    scanf("%s", buffer);
    int option = (int) strtol(buffer, (char **)NULL, 10);;
    return option;
}

static void client_subscribe() {
    printf("\nSubscribe to topic (blank for cancelling): ");
}

static void client_send() {
    printf("\nTopic (blank for cancelling): ");
    printf("\nMessage (blank for cancelling): ");
    //TODO
}

static void client_receive() {
    printf("\nLooking for messages...");
    //TODO
}

int main(int argc, char* argv[]) {

    log_info("Starting client.");

    while (!quit) {
        printf("\nWelcome to the message queue client!\n\n");
        printf("1. Subscribe\n");
        printf("2. Send\n");
        printf("3. Receive\n");
        printf("4. Quit\n");

        int option = read_option();
        if (option == 1) {
            client_subscribe();
        } else
        if (option == 2) {
            client_send();
        } else
        if (option == 3) {
            client_receive();
        } else
        if (option == 4) {
            printf("Goodbye\n");
            break;
        } else {
            printf("Invalid option.\n");
        }
    }

}