#ifndef TRAIN_XWAY_INCLUDE_TRAIN_CONTROL_H_
#define TRAIN_XWAY_INCLUDE_TRAIN_CONTROL_H_

#include <stdlib.h>
#include <semaphore.h>

#define NB_TRAINS 3
#define NB_SECTIONS 100

#define CHECK(status, msg)  \
	if (-1 == (status)) {     \
		perror(msg);            \
		exit(EXIT_FAILURE);     \
	}

#define CHECK_T(status, msg)                        \
  if (0 != (status))   {                            \
    fprintf(stderr, "pthread erreur : %s\n", msg);  \
    exit (EXIT_FAILURE);                            \
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
} SectionLocking;

typedef struct{
    pthread_cond_t cond[NB_TRAINS];
    pthread_mutex_t mutex[NB_TRAINS];
    int wu_flag[NB_TRAINS];
} ThreadControl;

int init_train(int train_id);

#endif  // TRAIN_XWAY_INCLUDE_TRAIN_CONTROL_H_

