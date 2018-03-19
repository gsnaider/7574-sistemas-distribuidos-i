#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

#include "include/shm.h"
#include "include/msg.h"
#include "include/semaforo.h"

int buffer[] = {66, 105, 101, 110, 118,
                101, 110, 105, 100, 111,
                115, 32, 97, 32, 83, 105,
                115, 116, 101, 109, 97,
                115, 32, 68, 105, 115,
                116, 114, 105, 98, 117,
                105, 100, 111, 115, 32,
                50, 48, 49, 56};

int main(int argc, char** argv){
    int i=0;
	//habia q agregar sizeof(int)
    int total = sizeof(buffer) / sizeof(int);

    int shm = creashm(0, sizeof(int)*2);
    int* memoria = (int*)map(shm);
    memoria[0]=0;
    memoria[1]=total;

    int sem = creasem(0);
    inisem(sem, 1);

    int wait_sem = creasem(1);
	// deberia inicializarse en cero.
    inisem(wait_sem, 0);

    int cola = creamsg(0);
	// deberia ser <
    for(i=0;i<total;i++){
        if(fork()==0){
            char number_str[3];

            int number = buffer[i];
            snprintf(number_str, 2,"%d", number);
            if(number%2) {
		// smprintf para convertir i a string
                execl("./procesar_2n", "./procesar_2n", i, number_str, (char*) NULL);
		printf("Error en exec par\n");
		exit(1);
	}
            else{
                execl("./procesar_n", "./procesar_n", smprintf("%d",i), number_str, (char*) NULL);
		printf("Error en exec impar\n");
		exit(1);
		}
        }
    }

    //Espero a que terminen todos
    p(wait_sem);
    printf("\n");

    return 0;
}
