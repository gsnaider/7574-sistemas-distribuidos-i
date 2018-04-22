#include "client.h"

int create();

int publish(int id, char *msg, char *topic);

int subscribe(int id, char *topic);

payload_t receive(int id);

int destroy(int id);