#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <semaphore.h>

#include "xway_tcp.h"
#include "xway_pkg.h"

char ip_address[12] = "10.31.125.14";

// Open a TCP connection
void open_connection(int *sock, struct sockaddr_in *server) {
  *sock = socket(AF_INET, SOCK_STREAM, 0);
  CHECK(*sock, "socket()");
  
  server->sin_addr.s_addr = inet_addr(ip_address);
  server->sin_family = AF_INET;
  server->sin_port = htons(PORT);

  CHECK(connect(*sock, (struct sockaddr *)server, sizeof(*server)), "connect()");
}

void close_connection(int *sock) {
  close(*sock);
}

void send_xway(int *sock, XwayPacket *xpck) {
  size_t len = xpck->len;
  unsigned char *message = xpck_combine_header_body(xpck);
  CHECK(write(*sock, message, xpck->len), "write()");
}

XwayPacket * receive_xway(int *sock) {
  char server_reply[MAX_CHAR];
  CHECK(read(*sock, server_reply, MAX_CHAR), "read()");
  XwayPacket *xpck = xpck_from_bytes(server_reply);
  return xpck;
}

void send_start(int *sock) {
  char start_bytes[] = {0x24, 0x06};
  XwayPacket *xpck = xpck_create_3_way(start_bytes);
  xpck_print(xpck);
  send_xway(sock, xpck);

  xpck = receive_xway(sock);
  xpck_print(xpck);
}

void send_stop(int *sock) {
  char stop_bytes[] = {0x25, 0x06};
  XwayPacket *xpck = xpck_create_3_way(stop_bytes);
  xpck_print(xpck);
  send_xway(sock, xpck);

  xpck = receive_xway(sock);
  xpck_print(xpck);
}

void send_order(int *sock, XwayPacket *xpck) {
  //printf("Sending order {\n");

  send_xway(sock, xpck);
  //printf("\tSend: "); xpck_print(xpck);

  XwayPacket *xpck_a = receive_xway(sock);
  //printf("\tRecv: "); xpck_print(xpck_a);

  if (xpck_a->body != NULL && xpck_a->body[0] == (unsigned char)0xfd) {
    //printf("\tRequirement already satisfied\n");
  }
  else if (xpck_a->body != NULL && xpck_a->body[0] == (unsigned char)0xfe) {
    XwayPacket *xpck_c = receive_xway(sock);
    //printf("\tRecv: "); xpck_print(xpck_c);
    unsigned char id = xpck_get_5_way_id(xpck_c);

    char bytes[] = { 0xfe };
    XwayPacket *xpck_r = xpck_create_5_way(bytes);
    xpck_set_5_way_id(xpck_r, id);
    xpck_r->header[FWAY_TYPE_BYTE] = (unsigned char)0x19;

    send_xway(sock, xpck_r);
    //printf("\tSend: "); xpck_print(xpck_r);

    xpck_destroy(xpck_c);
    xpck_destroy(xpck_r);
  } else {
    //printf("Incorrect message received.\n");
  }

  xpck_destroy(xpck_a);

  //printf("}\n");
}

void send_order_sem(int *sock, XwayPacket *xpck, sem_t * msg) {
  int addr = xpck->header[CLIENT_ADDR_BYTE];
  sem_wait(msg);
  //printf("Sending order {\n");
  
  send_xway(sock, xpck);
  //printf("\tSend: "); xpck_print(xpck);

  XwayPacket *xpck_a = receive_xway(sock);
  //printf("\tRecv: "); xpck_print(xpck_a);
  sem_post(msg);

  if (xpck_a->body != NULL && xpck_a->body[0] == (unsigned char)0xfd) {
    //printf("\tRequirement already satisfied\n");
  }
  else if (xpck_a->body != NULL && xpck_a->body[0] == (unsigned char)0xfe) {
    XwayPacket *xpck_c = receive_xway(sock);
    //printf("\tRecv: "); xpck_print(xpck_c);
    unsigned char id = xpck_get_5_way_id(xpck_c);

    char bytes[] = { 0xfe };
    XwayPacket *xpck_r = xpck_create_5_way(bytes);
    xpck_set_5_way_id(xpck_r, id);
    xpck_r->header[FWAY_TYPE_BYTE] = (unsigned char)0x19;
    xpck_r->header[CLIENT_ADDR_BYTE] = addr;

    sem_wait(msg);
    send_xway(sock, xpck_r);
    sem_post(msg);
    //printf("\tSend: "); xpck_print(xpck_r);

    xpck_destroy(xpck_c);
    xpck_destroy(xpck_r);
  } else {
    //printf("Incorrect message received.\n");
  }

  xpck_destroy(xpck_a);

  //printf("}\n");
}

