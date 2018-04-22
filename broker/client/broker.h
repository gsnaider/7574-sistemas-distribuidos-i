#ifndef BROKER_H
#define BROKER_H

#include "../common/message.h"

int creabrk();

int publish(int id, char *msg, char *topic);

int subscribe(int id, char *topic);

payload_t receive(int id);

int delbrk(int id);

#endif
