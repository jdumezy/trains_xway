#ifndef TRAIN_XWAY_INCLUDE_COMMANDE_TRAIN_H_
#define TRAIN_XWAY_INCLUDE_COMMANDE_TRAIN_H_

#include <stdlib.h>

#define CHECK(status, msg)  \
	if (-1 == (status)) {     \
		perror(msg);            \
		exit(EXIT_FAILURE);     \
	}

typedef void * (*pf_t)(void *);

int init_train(int train_id);

#endif  // TRAIN_XWAY_INCLUDE_COMMANDE_TRAIN_H_

