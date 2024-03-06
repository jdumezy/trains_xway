#include <arpa/inet.h>

#include "xway_tcp.h"
#include "xway_pkg.h"

#define TRACK_1_LEN 5
#define TRACK_2_LEN 6
#define TRACK_3_LEN 9
#define TRACK_4_LEN 6

// Loop
int track_1[] = { 3, 23, 10, 29, 19 };
int aig_1[] = {0};
// Loop
int track_2[] = { 4, 22, 27, 28, 9, 24 };
int aig_2[] = {0};
// Loop
int track_3[] = { 0, 13, 20, 30, 9, 31, 26, 15, 12 };
int aig_3[] = {0};
// In-out
int track_4[] = { 7, 29, 9, 28, 27, 47 };
int aig_4[] = {0};

int main() {
  int sock;
  struct sockaddr_in server;

  open_connection(&sock, &server);

  for (int i = 0; i < 5; i++) {
    XwayPacket *xpck = xpck_train_req(50, 39, track_1[i % TRACK_1_LEN]);
    send_order(&sock, xpck);
  }
  
  XwayPacket *xpck = xpck_aig_req(50, 39, 3);
  send_order(&sock, xpck);

  close_connection(&sock);
}

