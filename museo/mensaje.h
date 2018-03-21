#define ACEPTADO 1
#define RECHAZADO 2
#define SOLICITO_ENTRAR 3


struct t_mensaje {
    long mtype;
    int tipo_mensaje;
};

typedef struct t_mensaje mensaje_t;

