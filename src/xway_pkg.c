#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "xway_pkg.h"

char header_3_way[] = {0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0xF0, 0x00, 0x10, 0x00, 0x10};
char header_5_way[] = {0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0xF1, 0x00, 0x10, 0x00, 0x10, 0x09, 0x00};

#define REQ_LEN 14
char request[] = {0x37, 0x06, 0x68, 0x07, 0x00, 0x00, 0x03, 0x00, 0x30, 0x00, 0xff, 0xff, 0xff, 0xff};

// Get packet length
int length_packet(char *bytes) {
  int length = (SIZE_BYTE+1) + (unsigned char)bytes[SIZE_BYTE];
  return length;
}

// Set the length in the header
void xpck_set_length_in_header(XwayPacket *xpck) {
  if (xpck->header == NULL) {
    printf("Uninitialised Xway packet!\n");
  } else {
    xpck->header[SIZE_BYTE] = xpck->len - (SIZE_BYTE+1);
  }
}

// Print an Xway packet
void xpck_print(XwayPacket *xpck) {
  size_t length = xpck->len;
  printf("Xway [%zu]: ", length);
  size_t header_len = xpck->is_5_way ? HEADER_5_LEN : HEADER_3_LEN;

  for(size_t i = 0; i < length; ++i) {
    if (i == header_len) {
      printf("| ");
    } 
    if (i < header_len) {
      printf("%02X ", (xpck->header)[i]);
    } else {
      printf("%02X ", (xpck->body)[i - header_len]);
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
  size_t len = strlen(bytes);
  unsigned char *body = malloc(sizeof(unsigned char) * len);
  
  memcpy(body, bytes, len);

  xpck->body = body;
  xpck->len += len;
}

// Creates the body of an Xway packet
void xpck_set_body_n(XwayPacket *xpck, char *bytes, size_t len) {
  unsigned char *body = malloc(sizeof(unsigned char) * len);
  
  memcpy(body, bytes, len);

  xpck->body = body;
  xpck->len += len;
}

// Appends to the body of an Xway packet
void xpck_append_body(XwayPacket *xpck, char *bytes) {
  if (xpck->body == NULL) {
    printf("Uninitialized Xway packet!\n");
    exit(-1);
  }

  size_t len = strlen(bytes);

  size_t header_len = (xpck->is_5_way ? HEADER_5_LEN : HEADER_3_LEN);
  size_t current_body_len = xpck->len - header_len;
  size_t new_body_len = current_body_len + len;

  unsigned char *new_body = malloc(new_body_len);

  memcpy(new_body, xpck->body, current_body_len);
  memcpy(new_body + current_body_len, bytes, len);

  free(xpck->body);
  xpck->body = new_body;
  xpck->len = header_len + new_body_len;
}

// Destroy an Xway packet
void xpck_destroy(XwayPacket *xpck) {
  if (xpck->header != NULL) {
    free(xpck->header);
  }
  if (xpck->body != NULL) {
    free(xpck->body);
  }
}

// Create empty 3 way Xway packet
XwayPacket * xpck_create_3_way_empty() {
  XwayPacket * xpck = malloc(sizeof(XwayPacket));
  xpck_set_header(xpck, false);
  return xpck;
}

// Create 3 way Xway packet
XwayPacket * xpck_create_3_way(char *bytes) {
  XwayPacket * xpck = xpck_create_3_way_empty();
  xpck_set_body(xpck, bytes);
  xpck_set_length_in_header(xpck);
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
  XwayPacket * xpck = xpck_create_5_way_empty(); 
  xpck_set_body(xpck, bytes);
  xpck_set_length_in_header(xpck);
  return xpck;
}

// Create xpck from bytes
XwayPacket * xpck_from_bytes(char *bytes) {
  XwayPacket * xpck = malloc(sizeof(XwayPacket));

  int len = length_packet(bytes);
  xpck->len = len;

  bool is_5_way = (bytes[MSG_TYPE_BYTE] == (char)0xf1);

  int header_len = is_5_way ? HEADER_5_LEN : HEADER_3_LEN;
  int body_len = len - header_len;

  unsigned char *header = malloc(sizeof(char) * header_len);
  unsigned char *body = malloc(sizeof(char) * body_len);

  memcpy(header, bytes, header_len);
  memcpy(body, bytes + header_len, body_len);

  xpck->header = header;
  xpck->body = body;
  xpck->is_5_way = is_5_way;

  return xpck;
}

// Create request for a train
XwayPacket * xpck_train_req(int train_id, int addr_word, int addr_section) {
  XwayPacket * xpck = malloc(sizeof(XwayPacket));
  xpck = xpck_create_5_way_empty();
  xpck_set_5_way_id(xpck, train_id);
  xpck_set_body_n(xpck, request, REQ_LEN);
  xpck->body[REQ_ADDR_BYTE] = addr_word;
  xpck->body[REQ_SEC_BYTE] = addr_section;
  xpck->body[REQ_SEC_BYTE+1] = 0;
  xpck_set_length_in_header(xpck);
  return xpck;
}

// Create request for a rail switch
XwayPacket * xpck_aig_req(int train_id, int addr_word, int addr_aig) {
  XwayPacket * xpck = malloc(sizeof(XwayPacket));
  xpck = xpck_create_5_way_empty();
  xpck_set_5_way_id(xpck, train_id);
  xpck_set_body_n(xpck, request, REQ_LEN);
  xpck->body[REQ_ADDR_BYTE] = addr_word;
  xpck->body[REQ_AIG_1] = addr_aig;
  xpck->body[REQ_AIG_2] = 0;
  xpck_set_length_in_header(xpck);
  return xpck;
}

unsigned char* xpck_combine_header_body(const XwayPacket *xpck) {
  size_t header_len = xpck->is_5_way ? HEADER_5_LEN : HEADER_3_LEN;
  size_t body_len = xpck->len - header_len;
  
  unsigned char *combined = malloc(xpck->len);
  if (!combined) {
    printf("Memory allocation failed!\n");
    return NULL;
  }
  
  memcpy(combined, xpck->header, header_len);
  memcpy(combined + header_len, xpck->body, body_len);
  
  return combined;
}
