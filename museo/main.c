#define _GNU_SOURCE

#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include "semaphore.h"
#include "include/msg.h"
#include "include/shm.h"
#include "mensaje.h"

#define CANT_PUERTAS 3
#define CAP_MUSEO 10

int main(int argc, char* argv[]) {
	
	int semid = sem_get("./main.c", 1);
	sem_init(semid, 0, 1);
	
	int shmid = creashm(0, sizeof(int));
	if (shmid < 0) {
		printf("ERROR: inicializar memoria compartida\n");
		_exit(-1);	
	}
	
	int* cant_personas = (int*)map(shmid);
	*cant_personas = CAP_MUSEO;
	
    for (int i = 0; i < CANT_PUERTAS; i++) {
		// Par es para recibir mensajes (desde la puerta)
		creamsg(2 * i);
		// Impar para emitir mensajes (desde la puerta)
		creamsg(2 * i + 1);

		char number_str[3];
		snprintf(number_str, 3,"%d", i);

		int pid = fork();
		if (pid < 0) {
			printf("ERROR\n");
			_exit(-1);
		}
		if (pid == 0) {
			execl("./puerta", "./puerta", number_str, (char*)NULL);
			printf("ERROR: failed execv\n");
			_exit(-1);
		}
	}
/*	
	int pid = fork();
	if (pid < 0) {
		printf("ERROR\n");
		_exit(-1);
	}
	if (pid == 0) {
		execl("./spawner", "./spawner", (char*)NULL);
		printf("ERROR: failed execv\n");
		_exit(-1);
	}
*/

	for(int i = 1; i < 30; i++){
		mensaje_t un_mensajito = {i, SOLICITO_ENTRAR};
		printf("Envio mensajito %d\n", i);
		enviarmsg(0, &un_mensajito, sizeof(mensaje_t));
	
		recibirmsg(1, &un_mensajito, sizeof(mensaje_t), 0);
		printf("Recibo mensajito %d con tipo %d\n", i, un_mensajito.tipo_mensaje);

	}
    for (int i = 0; i < CANT_PUERTAS; i++) {
		wait(NULL);
	}
}
	
