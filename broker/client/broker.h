#ifndef BROKER_H
#define BROKER_H

#include "../common/message.h"

int creabrk();

int publish(int id, char *message, char *topic);

int subscribe(int id, char *topic);

int receive(int id, payload_t* payload);

int delbrk(int id);

#endif
