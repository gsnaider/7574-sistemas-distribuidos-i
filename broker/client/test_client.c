//
// Created by gaston on 04/05/18.
//

#include "../common/log/log.h"
#include "broker.h"

int main(int argc, char* argv[]) {
    log_info("Starting test client.");
    int msg_brk = creabrk();
    log_info("id received: %d", msg_brk);
}
