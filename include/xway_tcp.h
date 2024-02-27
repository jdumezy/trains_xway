#ifndef TRAIN_XWAY_INCLUDE_XWAY_TCP_H_
#define TRAIN_XWAY_INCLUDE_XWAY_TCP_H_

#include <stdlib.h>
#include <arpa/inet.h>
#include <stdio.h>

#include "xway_pkg.h"

#define MAX_CHAR 1000
#define PORT 502

#define CHECK(status, msg)	\
	if (-1 == (status)) {	    \
		perror(msg);		        \
		exit(EXIT_FAILURE);	    \
	}

void open_connection(int *sock, struct sockaddr_in *server);
void close_connection(int *sock);
void send_xway(int *sock, XwayPacket *xpck);
XwayPacket * receive_xway(int *sock);
void send_start(int *sock);
void send_stop(int *sock);
void send_order(int *sock, XwayPacket *xpck);

#endif  // TRAIN_XWAY_INCLUDE_XWAY_TCP_H_
 
