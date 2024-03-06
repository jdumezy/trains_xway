#ifndef TRAIN_XWAY_INCLUDE_TRAIN_H_
#define TRAIN_XWAY_INCLUDE_TRAIN_H_

#include <stdlib.h>

#define NB_SEMAPHORES 14

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

#endif  // TRAIN_XWAY_INCLUDE_TRAIN_H_

