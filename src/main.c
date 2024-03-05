#include <arpa/inet.h>

#include "xway_tcp.h"
#include "xway_pkg.h"

// Loop
int track_1[] = {3, 23, 10, 29, 19};
// Loop
int track_2[] = {4, 22, 27, 28, 9, 24};
// Loop
int track_3[] = {0, 13, 20, 30, 9, 31, 26, 15, 12};
// In-out
int track_4[] = {7, 29, 9, 28, 27, 47};

int main() {
  int sock;
  struct sockaddr_in server;

  open_connection(&sock, &server);

  for (int i = 1; i < 20; i++) {
    XwayPacket *xpck = xpck_train_req(50, 39, track_1[i%5]);
    send_order(&sock, xpck);
  }

  close_connection(&sock);
}

