#include <stdlib.h>

#include "xway_pkg.h"

// Set len when received
void xpck_set_intern_length(XwayPacket *xpck) {
  if (xpck->header == NULL) {
    printf("Uninitialised Xway packet!\n");
  } else {
    unsigned int length = SIZE_BYTE+1 + (unsigned char)(xpck->header)[SIZE_BYTE];
    xpck->len = (size_t)length;
  }
}

// Set len in header
void xpck_set_pck_length(XwayPacket *xpck) {
  if (xpck->header == NULL) {
    printf("Uninitialised Xway packet!\n");
  } else {
    xpck->header[SIZE_BYTE] = xpck->len;
  }
}

// Print an Xway packet
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

// Checks if an Xway packet is 5 way
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

// Sets the header for a newly created Xway packet
void xpck_set_header(XwayPacket *xpck, bool is_5_way) {
  int len = is_5_way ? HEADER_5_LEN : HEADER_3_LEN;
  char *h = is_5_way ? header_5_way : header_3_way;

  unsigned char *header = malloc((sizeof(unsigned char)) * len);
  for (int i = 0; i < len; i++) {
    switch (i) {
      case CLIENT_ADDR_BYTE:
        header[i] = XWAY_CLIENT_ADDR;
        break;
      case SERVER_ADDR_BYTE:
        header[i] = XWAY_SERVER_ADDR;
        break;
      default:
        header[i] = h[i];
        break;
    }
  }

  xpck->header = header;
  xpck->len += len;
  xpck->is_5_way = is_5_way;
}

// Set 5 way id of an Xway packet
void xpck_set_5_way_id(XwayPacket *xpck, unsigned char id) {
  if (xpck->header == NULL) {
    printf("Uninitialised Xway packet!\n");
  }
  xpck->header[FWAY_ID_BYTE] = id;
}

// Get 5 way id of an Xway packet
unsigned char xpck_get_5_way_id(XwayPacket *xpck) {
  if (xpck->header == NULL) {
    printf("Uninitialised Xway packet!\n");
    return (unsigned char) 0x00;
  }
  return xpck->header[FWAY_ID_BYTE];
}

// Creates the body of an Xway packet
void xpck_set_body(XwayPacket *xpck, char *bytes) {
  size_t len = sizeof(bytes);
  unsigned char *body = malloc(sizeof(unsigned char) * len);
  for (size_t i = len; i < len; i++) {
    body[i] = bytes[i];
  }
  xpck->body = body;
  xpck->len += len;
}

// Appends to the body of an Xway packet
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

// Destroy an Xway packet
void xpck_destroy(XwayPacket *xpck) {
  if (xpck->header != NULL) {
    free(xpck->header);
  }
  if (xpck->body != NULL) {
    free(xpck->body);
  }
  free(xpck);
}

// Create empty 3 way Xway packet
XwayPacket * xpck_create_3_way_empty() {
  XwayPacket * xpck = malloc(sizeof(XwayPacket));
  xpck_set_header(xpck, false);
  return xpck;
}

// Create 3 way Xway packet
XwayPacket * xpck_create_3_way(char *bytes) {
  XwayPacket * xpck = malloc(sizeof(XwayPacket));
  xpck_set_header(xpck, false);
  xpck_set_body(xpck, bytes);
  return xpck;
}

// Create empty 5 way Xway packet
XwayPacket * xpck_create_5_way_empty() {
  XwayPacket * xpck = malloc(sizeof(XwayPacket));
  xpck_set_header(xpck, true);
  return xpck;
}

// Create 5 way Xway packet
XwayPacket * xpck_create_5_way(char *bytes) {
  XwayPacket * xpck = malloc(sizeof(XwayPacket));
  xpck_set_header(xpck, true);
  xpck_set_body(xpck, bytes);
  return xpck;
}

XwayPacket * xpck_from_bytes(char *bytes) {
  XwayPacket * xpck = malloc(sizeof(XwayPacket));
  // à faire
  return xpck;
}

// when receiving a xway packet:
// set len, set 5 way (if 5 way get id)), eventually print, get key
// add function to switch byte from 0 to 1;
