#include <arpa/inet.h>

#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <unistd.h>

#include "xway_tcp.h"
#include "xway_pkg.h"
#include "train.h"

// Loop
int track_1[] = { 3, 23, 10, 29, 19 };
int aig_1[] =   {12, 10, 13,  2, -1};
// Loop
int track_2[] = { 4, 22, 27, 28, 9, 24 };
int aig_2[] =   {-1,  3, -1,  6, 5,  8 };
// Loop
int track_3[] = { 13, 20, 30, 11, 31, 26, 15, 12 };
int aig_3[] =   { -1, 10,  7,  6, -1,  9,  1, -1};
// In-out
int track_4[] = { 7, 29, 9, 28, 27, 47 };
int aig_4[] =   {-1, -1, -1, -1, -1, -1};

int *tracks[] = {track_1, track_2, track_3, track_4};
int *aig[] = {aig_1, aig_2, aig_3, aig_4};

int len_track[] = {5, 6, 9, 6};
int nb_laps[] = {1, 1, 1, 1};
int words[] = {39, 42, 50, 53};

//               0  1  2  3   4   5   6   7   8   9  10  11  12  13
int tab_aig[] = {0, 1, 3, 7, 10, 12, 13, 14, 20, 21, 22, 23, 31, 33};

sem_t semaphores[NB_SEMAPHORES];
sem_t msg;

void init_semaphores() {
    for (int i = 0; i < NB_SEMAPHORES; i++) {
        sem_init(&semaphores[i], 0, 1);
    }
  sem_init(&msg, 0, 1);
}

void destroy_semaphores() {
    for (int i = 0; i < NB_SEMAPHORES; i++) {
        sem_destroy(&semaphores[i]);
    }
}

void th_train_control(long idl) {
  int id = (int)idl;

  int lt = len_track[id];
  int nb_iter = nb_laps[id] * lt;

  int sock;
  struct sockaddr_in server;

  open_connection(&sock, &server);

  for (int i = 0; i < nb_iter; i++) {
    int aig_id = aig[id][i % lt];

    if (aig_id != -1) {
      int aig_val = tab_aig[aig_id];

      sem_wait(&semaphores[aig_id]);

      int a = words[id];

      XwayPacket *xpck_aig = xpck_aig_req(50+10*id, words[id], aig_val);
      send_order_sem(&sock, xpck_aig, &msg);
      xpck_destroy(xpck_aig);

      XwayPacket *xpck_cmd = xpck_train_req(50+10*id, words[id], tracks[id][i % lt]);
      send_order_sem(&sock, xpck_cmd, &msg);
      xpck_destroy(xpck_cmd);

      sem_post(&semaphores[aig_id]);
    } else {
      XwayPacket *xpck_cmd = xpck_train_req(50+10*id, words[id], tracks[id][i % lt]);
      send_order_sem(&sock, xpck_cmd, &msg);
      xpck_destroy(xpck_cmd);
    }
  }

  close_connection(&sock);
}

int main(int argc, char *argv[]) {
  int nb_trains = 0;
  int nb_tours[] = {0, 0, 0, 0};
  if (argc > 1) {
    nb_trains = atoi(argv[1]);
    if (argc - 2 == nb_trains && nb_trains < 5) {
      for (int i = 0; i < nb_trains; i++) {
        nb_laps[i] = atoi(argv[i+2]);
      }
    }
  } else {
      printf("Incorrect usage\n");
      exit(EXIT_FAILURE);
  }

  init_semaphores();

  pthread_t t_ctrl_id[nb_trains];

  for (int i = 0; i < nb_trains; i++) {
    printf("Launching thread %d\n", i);
    CHECK_T(pthread_create(&t_ctrl_id[i], NULL, (pf_t) th_train_control, (void *) (long) i), "pthread_create");
  }

  for (int i = 0; i < nb_trains; i++) {
    CHECK_T(pthread_join(t_ctrl_id[i], NULL), "pthread_join");
  }

  destroy_semaphores();
}

