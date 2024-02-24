#ifndef TRAIN_XWAY_INCLUDE_XWAY_MSG_H_
#define TRAIN_XWAY_INCLUDE_XWAY_MSG_H_

#include <stdio.h>
#include <stdbool.h>

#define MAX_CHAR 1000

#define SIZE_BYTE 5
#define MSG_TYPE_BYTE 7
#define ADDR_BYTE 8
#define FWAY_TYPE_BYTE 12
#define FWAY_ID_BYTE 13

#define HEADER_3_LEN 12
#define HEADER_5_LEN 14

#define XWAY_ADDR 48

typedef struct {
  unsigned char *header;
  unsigned char *body;
  size_t len;
  bool is_5_way;
} XwayPacket;

char header_3_way[] = {0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0xF0, 0x30, 0x10, 0x14, 0x10};
char header_5_way[] = {0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0xF1, 0x30, 0x10, 0x14, 0x10, 0x09, 0x00}; // change last byte with id
char stop[] = {0x25, 0x06};
char unstop[] = {0x24, 0x06};

#endif  // TRAIN_XWAY_INCLUDE_XWAY_MSG_H_

