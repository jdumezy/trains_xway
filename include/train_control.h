#ifndef TRAIN_XWAY_INCLUDE_COMMANDE_TRAIN_H_
#define TRAIN_XWAY_INCLUDE_COMMANDE_TRAIN_H_

#include <stdlib.h>
#include <semaphore.h>

#define NB_TRAINS 3
#define NB_SECTIONS 100

#define CHECK(status, msg)  \
	if (-1 == (status)) {     \
		perror(msg);            \
		exit(EXIT_FAILURE);     \
	}

typedef void * (*pf_t)(void *);

typedef struct {
  int speed[NB_TRAINS];
  sem_t mutex;
} TrainSpeed;

typedef struct {
  int sections[NB_SECTIONS];
  sem_t sem_sec[NB_SECTIONS];
  sem_t mutex;
} TrackSections;

int init_train(int train_id);

#endif  // TRAIN_XWAY_INCLUDE_COMMANDE_TRAIN_H_

