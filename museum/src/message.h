#define ENTRANCE_REQUEST 1
#define EXIT_REQUEST 2
#define ACCEPT 3
#define REJECT 4

typedef struct message_t {
    long mtype;
    int type;
} message_t;
