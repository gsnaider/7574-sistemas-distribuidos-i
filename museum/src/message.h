#define REQUEST 1
#define ACCEPT 2
#define REJECT 3

typedef struct message_t {
    long mtype;
    int type;
} message_t;
