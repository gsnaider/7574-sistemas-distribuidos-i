//
// Created by gaston on 25/05/18.
//

#ifndef BROKER_DB_MESSAGE_H
#define BROKER_DB_MESSAGE_H

#include "../server/server.h"
#include "../common/message.h"

#define DB_CREATE 1
#define DB_SUBSCRIBE 2
#define DB_GET_SUBSCRIBED 3
#define DB_DELETE 4

typedef struct db_msg_t {
    int global_id;
    int type;
    char topic[MAX_TOPIC_LENGTH];

    //TODO find a way to reply with a dynamic amount of ids (for the get_subscribed operation)
    int ids[MAX_CLIENTS];
} db_msg_t;

#endif //BROKER_DB_MESSAGE_H
