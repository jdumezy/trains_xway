#include <arpa/inet.h>

#include "xway_tcp.h"
#include "xway_pkg.h"

int main() {
  int sock;
  struct sockaddr_in server;

  //open_connection(&sock, &server);

  //close_connection(&sock);

  //printf("Connection ok\n");

  char stop_bytes[] = {0x25, 0x06};
  XwayPacket *xpck = xpck_create_3_way(stop_bytes);
  xpck_append_body(xpck, stop_bytes);
  xpck_print(xpck);
  printf("xpck create and print\n");
  xpck_destroy(xpck);
  //printf("xpck destroyed");

  char pkg[] = {0x00, 0x00, 0x00, 0x01, 0x00, 0x08, 0x00, 0xF0, 0x30, 0x10, 0x14, 0x10, 0x25, 0x06};

  XwayPacket *xpck2 = xpck_from_bytes(pkg);
  xpck_print(xpck2);
  xpck_destroy(xpck2);

  printf("End test\n");
}

