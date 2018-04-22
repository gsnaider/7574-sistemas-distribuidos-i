//
// Created by gaston on 22/04/18.
//

#ifndef BROKER_SHM_H
#define BROKER_SHM_H

int   creashm(int,int);
int   getshm(int);
void* map(int);
int   unmap(void*);
int   delshm(int);

#endif //BROKER_SHM_H
