//
// Created by gaston on 22/04/18.
//

#ifndef BROKER_SEMAPHORE_H
#define BROKER_SEMAPHORE_H


/* Funciones de semaforos crear el set de semaforos (si no existe)
 */
int creasem(int identif);


/* adquirir derecho de acceso al set de semaforos existentes
 */
int getsem(int identif);


/* inicializar al semáforo del set de semaforos
 */
int inisem(int semid, int val);


/* ocupar al semáforo (p) WAIT
 */
int p(int semid);


/* liberar al semáforo (v) SIGNAL
 */
int v(int semid);


/* eliminar el set de semaforos
 */
int delsem(int semid);


#endif //BROKER_SEMAPHORE_H
