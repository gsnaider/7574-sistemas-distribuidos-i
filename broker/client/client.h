#ifndef BROKER_CLIENT_H
#define BROKER_CLIENT_H

#include "../common/message.h"

int create();

int publish(int id, char *msg, char *topic);

int subscribe(int id, char *topic);

payload_t receive(int id);

int destroy(int id);

#endif
