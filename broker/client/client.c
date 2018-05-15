//
// Created by gaston on 15/05/18.
//

#include <stdbool.h>
#include <unistd.h>
#include <stdlib.h>
#include "../common/log/log.h"
#include "broker.h"

bool quit = false;

static int read_option() {
    char buffer[10];
    scanf("%s", buffer);
    int option = (int) strtol(buffer, (char **)NULL, 10);;
    return option;
}



static void client_subscribe(int mom) {
    char topic[MAX_TOPIC_LENGTH];
    printf("\nSubscribe to topic: ");
    scanf("%s", topic);
    subscribe(mom, topic);
}

static void client_send(int mom) {
    printf("\nTopic: ");
    printf("\nMessage: ");
    //TODO
}

static void client_receive(int mom) {
    printf("\nLooking for messages...");
    //TODO
}

int main(int argc, char* argv[]) {

    log_info("Starting client.");
    int mom = creabrk();
    log_info("mom created: %d", mom);

    printf("\n\nWelcome to the message queue client!\n\n");
    while (!quit) {
        printf("1. Subscribe\n");
        printf("2. Send\n");
        printf("3. Receive\n");
        printf("4. Quit\n");

        int option = read_option();
        if (option == 1) {
            client_subscribe(mom);
        } else
        if (option == 2) {
            client_send(mom);
        } else
        if (option == 3) {
            client_receive(mom);
        } else
        if (option == 4) {
            printf("Goodbye\n");
            break;
        } else {
            printf("Invalid option.\n");
        }
    }

    log_info("Stopping read client.");
    log_info("Deleting mom.");
    log_info("Delete result %d.", delbrk(mom));
}