#ifndef TRAIN_XWAY_INCLUDE_XWAY_MSG_H_
#define TRAIN_XWAY_INCLUDE_XWAY_MSG_H_

#include <stdbool.h>
#include <stdlib.h>

#define SIZE_BYTE 5
#define MSG_TYPE_BYTE 7
#define CLIENT_ADDR_BYTE 8
#define SERVER_ADDR_BYTE 10
#define FWAY_TYPE_BYTE 12
#define FWAY_ID_BYTE 13

#define HEADER_3_LEN 12
#define HEADER_5_LEN 14

#define XWAY_CLIENT_ADDR 48
#define XWAY_SERVER_ADDR 20

typedef struct {
  unsigned char *header;
  unsigned char *body;
  size_t len;
  bool is_5_way;
} XwayPacket;

void xpck_set_intern_length(XwayPacket *xpck);
void xpck_print(XwayPacket *xpck);
void xpck_set_is_5_way(XwayPacket *xpck);
void xpck_set_header(XwayPacket *xpck, bool is_5_way);
void xpck_set_5_way_id(XwayPacket *xpck, unsigned char id);
unsigned char xpck_get_5_way_id(XwayPacket *xpck);
void xpck_set_body(XwayPacket *xpck, char *bytes);
void xpck_append_body(XwayPacket *xpck, char *bytes);
void xpck_destroy(XwayPacket *xpck);
XwayPacket * xpck_create_3_way_empty();
XwayPacket * xpck_create_3_way(char *bytes);
XwayPacket * xpck_create_5_way_empty();
XwayPacket * xpck_create_5_way(char *bytes);
XwayPacket * xpck_from_bytes(char *bytes);

#endif  // TRAIN_XWAY_INCLUDE_XWAY_MSG_H_

