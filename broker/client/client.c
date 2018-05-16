//
// Created by gaston on 15/05/18.
//

#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include "../common/log/log.h"
#include "broker.h"

bool quit = false;

static void read_input(char* buffer, int max_size) {
    fgets(buffer, max_size, stdin);
    //Remove trailing newline char.
    if (strlen(buffer) > 0 && buffer[strlen(buffer) - 1] == '\n') {
        buffer[strlen(buffer) - 1] = '\0';
    }
}

static int read_option() {
    char buffer[10];
    read_input(buffer, 10);
    int option = (int) strtol(buffer, (char **)NULL, 10);;
    return option;
}

static void client_subscribe(int mom) {
    char topic[MAX_TOPIC_LENGTH];
    printf("\nSubscribe to topic: ");
    read_input(topic, MAX_TOPIC_LENGTH);
    subscribe(mom, topic);
}

static void client_send(int mom) {
    char topic[MAX_TOPIC_LENGTH];
    printf("\nTopic: ");
    read_input(topic, MAX_TOPIC_LENGTH);

    char message[MAX_MSG_LENGTH];
    printf("Message: ");
    read_input(message, MAX_MSG_LENGTH);

    publish(mom, message, topic);
}

static void client_receive(int mom) {
    printf("\nLooking for messages...");
    payload_t payload;
    int res = receive(mom, &payload);
    if (res > 0) {
        printf("\nMessage received on topic '%s' : '%s'\n", payload.topic, payload.msg);
    } else if (res == 0) {
        printf("\nNo new messages.\n");
    }
}

int main(int argc, char* argv[]) {

    log_info("Starting client.");
    int mom = creabrk();
    log_info("mom created: %d", mom);

    printf("\n\nWelcome to the message queue client!\n\n");
    while (!quit) {
        printf("\n1. Subscribe\n");
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
            printf("\nInvalid option.\n");
        }
    }

    log_info("Stopping read client.");
    log_info("Deleting mom.");
    log_info("Delete result %d.", delbrk(mom));
}