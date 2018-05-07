//
// Created by gaston on 04/05/18.
//

#include <unistd.h>
#include "../common/log/log.h"
#include "broker.h"

int main(int argc, char* argv[]) {
    log_info("Starting test client write.");
    int mom = creabrk();
    log_info("mom created: %d", mom);

    int res = publish(mom, "Infinity war rules!", "Avengers");
    log_info("Published message result %d.", res);

    res = delbrk(mom);
    log_info("Delete result %d.", res);

}
