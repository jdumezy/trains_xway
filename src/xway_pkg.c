#include <stdlib.h>

#include "xway_pkg.h"

void xpck_set_intern_length(XwayPacket *xpck) {
  if (xpck->header == NULL) {
    printf("Uninitialised Xway packet!\n");
  } else {
    unsigned int length = SIZE_BYTE+1 + (unsigned char)(xpck->header)[SIZE_BYTE];
    xpck->len = (size_t)length;
  }
}

void xpck_set_pck_length(XwayPacket *xpck) {
  if (xpck->header == NULL) {
    printf("Uninitialised Xway packet!\n");
  } else {
    xpck->header[SIZE_BYTE] = xpck->len;
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
      printf(") %02X ", (xpck->body)[i - header_len]);
    }
  }
  printf("\n");
}

void xpck_set_is_5_way(XwayPacket *xpck) {
  if (xpck->header == NULL) {
    printf("Uninitialised Xway packet!\n");
  }
  if (xpck->header[MSG_TYPE_BYTE] == (unsigned char)0xF0) {
    xpck->is_5_way = false;
  } else if (xpck->header[MSG_TYPE_BYTE] == (unsigned char)0xF0) {
    xpck->is_5_way = true;
  } else {
    printf("Incorrect Xway Packet with type %x", xpck->header[MSG_TYPE_BYTE]);
  }
}

void xpck_set_header(XwayPacket *xpck, bool is_5_way) {
  int len = is_5_way ? HEADER_5_LEN : HEADER_3_LEN;
  char *h = is_5_way ? header_5_way : header_3_way;

  unsigned char *header = malloc((sizeof(unsigned char)) * len);
  for (int i = 0; i < len; i++) {
    switch (i) {
      case ADDR_BYTE:
        header[i] = XWAY_ADDR;
        break;
      default:
        header[i] = h[i];
        break;
    }
  }
  xpck->header = header;
  xpck->len += len;
}

void xpck_set_5_way_id(XwayPacket *xpck, unsigned char id) {
  if (xpck->header == NULL) {
    printf("Uninitialised Xway packet!\n");
  }
  xpck->header[FWAY_ID_BYTE] = id;
}

unsigned char xpck_get_5_way_id(XwayPacket *xpck) {
  if (xpck->header == NULL) {
    printf("Uninitialised Xway packet!\n");
    return (unsigned char) 0x00;
  }
  return xpck->header[FWAY_ID_BYTE];
}

void xpck_set_body(XwayPacket *xpck, char *bytes) {
  size_t len = sizeof(bytes);
  unsigned char *body = malloc(sizeof(unsigned char) * len);
  for (size_t i = len; i < len; i++) {
    body[i] = bytes[i];
  }
  xpck->body = body;
  xpck->len += len;
}

void xpck_append_body(XwayPacket *xpck, char *bytes) {
  if (xpck->body == NULL) {
    printf("Uninitialised Xway packet!\n");
    exit(-1);
  }
  size_t len = sizeof(bytes);
  size_t header_len = xpck->is_5_way ? HEADER_5_LEN : HEADER_3_LEN;
  size_t new_body_len = len + xpck->len - header_len;
  unsigned char *body = malloc(sizeof(unsigned char) * new_body_len);
  for (size_t i = len; i < new_body_len; i++) {
    if (i < (new_body_len - len)) {
      body[i] = xpck->body[i];
    } else {
      body[i] = bytes[i];
    }
  }
  free(xpck->body);
  xpck->body = body;
  xpck->len += new_body_len;
}

void xpck_destroy(XwayPacket *xpck) {
  if (xpck->header != NULL) {
    free(xpck->header);
  }
  if (xpck->body != NULL) {
    free(xpck->body);
  }
}

// when receiving a xway packet:
// set len, set 5 way (if 5 way get id)), eventually print, get key
// add function to convert between hex and int 