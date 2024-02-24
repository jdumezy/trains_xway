#include <stdatomic.h>
#include <stdlib.h>

#include "xway_msg.h"

void xpck_set_length(XwayPacket *xpck) {
  if (xpck->header == NULL) {
    printf("Uninitialised Xway packet!\n");
  } else {
    unsigned int length = SIZE_BYTE + (unsigned char)(xpck->header)[5];
    xpck->len = (size_t)length;
  }
}

void xpck_print(XwayPacket *xpck) {
  size_t length = xpck->len;
  printf("Xway [%zu]: (", length);
  size_t header_len = xpck->is_5_way ? HEADER_5_LEN : HEADER_3_LEN;

  for(size_t i = 0; i < length; ++i) {
    if (i < header_len) {
      printf("%02X ", (xpck->header)[i]);
    } else {
      printf(") %02X ", (xpck->body)[i % header_len]);
    }
  }
  printf("\n");
}

void xpck_set_is_5_way(XwayPacket *xpck) {
  if (xpck->header == NULL) {
    printf("Uninitialised Xway packet!\n");
  }
  if (xpck->header[7] == (unsigned char)0xF0) {
    xpck->is_5_way = false;
  } else if (xpck->header[7] == (unsigned char)0xF0) {
    xpck->is_5_way = true;
  } else {
    printf("Incorrect Xway Packet with type %x", xpck->header[7]);
  }
}

char* create_req(char *bytes) {
  char* result = malloc(sizeof(char) * (HEADER_LEN + sizeof(bytes)));
  for (int i = 0; i < HEADER_LEN; i++) {
    result[i] = header[i];
  }
  for (int i = 0; i < sizeof(bytes); i++) {
    result[i+HEADER_LEN] = header[i];
  }
  return result;
}
