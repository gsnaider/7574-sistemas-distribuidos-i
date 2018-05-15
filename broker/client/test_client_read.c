//
// Created by gaston on 04/05/18.
//

#include <unistd.h>
#include <stdbool.h>
#include "../common/log/log.h"
#include "broker.h"
#include "../common/ipc/sig.h"


bool graceful_quit = false;

void SIGINT_handler(int signum) {
    if (signum != SIGINT) {
        log_warn("WARNING: Unknown signal received: %d.", signum);
    } else {
        log_debug("SIGINT received, aborting.");
        graceful_quit = true;
    }
}

int main(int argc, char* argv[]) {

    register_handler(SIGINT_handler);

    log_info("Starting test client read.");
    int mom = creabrk();
    log_info("mom created: %d", mom);

    int res = subscribe(mom, "marvel");
    log_info("Subscribed result %d.", res);

    while (!graceful_quit) {
        payload_t payload;
        res = receive(mom, &payload);
        log_info("Receive result %d.", res);
        log_info("Message received on topic '%s' : '%s'", payload.topic, payload.msg);
    }

    log_info("Stopping read client.");
    res = delbrk(mom);
    log_info("Delete result %d.", res);

}
