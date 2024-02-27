#ifndef TRAIN_XWAY_INCLUDE_XWAY_PACKET_H_
#define TRAIN_XWAY_INCLUDE_XWAY_PACKET_H_

#include <stdbool.h>
#include <stdio.h>

typedef struct {
  unsigned char *header;
  unsigned char *body;
  size_t len;
  bool is_5_way;
} XwayPacket;

#endif  // TRAIN_XWAY_INCLUDE_XWAY_PACKET_H_

