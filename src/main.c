#include <arpa/inet.h>

#include "xway_tcp.h"

int main() {
  int sock;
  struct sockaddr_in server;

  open_connection(&sock, &server);

  close_connection(&sock);
}

