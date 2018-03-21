#define _GNU_SOURCE 

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "mensaje.h"
#include <sys/types.h>
#include "semaphore.h"
#include "include/msg.h"
#include "include/shm.h"

int main(int argc, char* argv[]) {
	
	int semid = sem_get("./main.c", 1);

	int shmid = getshm(0);
	if (shmid < 0) {
		printf("ERROR: get memoria compartida\n");
		_exit(-1);	
	}
	int* cant_personas = (int*)map(shmid);

	int num_puerta;
    sscanf(argv[1], "%d", &num_puerta);

	printf("%d: soy la puerta %d\n", getpid(), num_puerta);    

	int cola_entrada = getmsg(2 * num_puerta);
	int cola_salida = getmsg(2 * num_puerta + 1);


	for (int i = 0; i < 30; i++) {
	
		mensaje_t solicitud;
		// Recibir mensaje
		recibirmsg(cola_entrada, &solicitud, sizeof(mensaje_t), 0);
				
		sleep(1);
		
		sem_wait(semid, 0);	
		int entra = 0;	
		if (*cant_personas > 0) {
			*cant_personas -= 1;
			entra = 1;
		}
		sem_post(semid, 0);		
		
		if (entra) {
			printf("%d: Entro alguien\n", getpid());
			// responde ok
			solicitud.tipo_mensaje = ACEPTADO;
			
		} else {
			printf("%d: Alguien fue rechazado\n", getpid());
			// responde afuera
			solicitud.tipo_mensaje = RECHAZADO;
		}
		
		enviarmsg(cola_salida, &solicitud, sizeof(mensaje_t));
	}
	
}
