#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <signal.h>

#include "train_control.h"

TrainSpeed *train_speed;
SectionLocking *section_locking;
ThreadControl *thread_control;

volatile sig_atomic_t stop_flag = 0;

void sigint_handler(int sig) {
  if (sig == SIGINT) {
    stop_flag = 1;
  }
}

// Communication API/path functions
int get_section_id();
int get_next_section_id();
int lock_and_set_section(int section_id, int train_id);// be careful with multilock/multiunlock for critical sections
int send_uspeed(int train_id, int speed);
//int is_section_switch(int section_id);

// pthread_cond_signal(&cond); // Wake up a waiting thread

void th_train_control(long idl) {
  int id = (int)idl;

  int section = get_section_id();
  sem_wait(&(section_locking->sem_sec[section]));
  lock_and_set_section(section, id);

  int next_section = get_next_section_id();
  sem_wait(&(section_locking->sem_sec[next_section]));
  lock_and_set_section(next_section, id);
  
  sem_wait(&(train_speed->mutex));
  train_speed->speed[id] = 1;
  sem_post(&(train_speed->mutex));

  while(!stop_flag) {
    pthread_mutex_lock(&(thread_control->mutex[id]));
    while (!(thread_control->wu_flag[id])) {
      pthread_cond_wait(&(thread_control->cond[id]), &(thread_control->mutex[id]));
    }

    // Free previous section and get new one
    sem_post(&(section_locking->sem_sec[section]));
    section = next_section;
    next_section = get_next_section_id();

    // Stops train while waiting section to be freed
    sem_wait(&(train_speed->mutex));
    train_speed->speed[id] = 0;
    sem_post(&(train_speed->mutex));

    sem_wait(&(section_locking->sem_sec[next_section]));
    lock_and_set_section(section, id);

    // Start train because section was acquired
    sem_wait(&(train_speed->mutex));
    train_speed->speed[id] = 1;
    sem_post(&(train_speed->mutex));
    
    pthread_mutex_unlock(&(thread_control->mutex[id]));
  }
}

// Function to interpret train_speed and periodically send uspeed
void thread_uspeed(long idl) {
  int id = (int)idl;

  while(!stop_flag) {
    usleep(100);
    sem_wait(&(train_speed->mutex));
    send_uspeed(id, train_speed->speed[id]);
    sem_post(&(train_speed->mutex));
  }
}

int main() {
  // Initialize variables
  train_speed = malloc(sizeof(TrainSpeed));
  section_locking = malloc(sizeof(SectionLocking));
  thread_control = malloc(sizeof(ThreadControl));

  sem_init(&train_speed->mutex, 0, 1);
  for (int i = 0; i < NB_TRAINS; i++) {
    train_speed->speed[i] = 0;
  }

  sem_init(&section_locking->mutex, 0, 1);
  for (int i = 0; i < NB_SECTIONS; i++) {
    sem_init(&section_locking->sem_sec[i], 0, 1);
    section_locking->sections[i] = -1;
  }

  for (int i = 0; i < NB_TRAINS; i++) {
    pthread_mutex_init(&thread_control->mutex[i], NULL);
    pthread_cond_init(&thread_control->cond[i], NULL);
    thread_control->wu_flag[i] = 0;
  }

  // Initialize signal handler
  struct sigaction sa;
  sa.sa_handler = sigint_handler;
  sigemptyset(&sa.sa_mask);
  sa.sa_flags = 0;
  CHECK(sigaction(SIGINT, &sa, NULL), "sigaction");

  // Create threads
  pthread_t t_ctrl_id[NB_TRAINS];

  for (int i = 0; i < NB_TRAINS; i++) {
    CHECK_T(pthread_create(&t_ctrl_id[i], NULL, (pf_t) th_train_control, (void *) (long) i), "pthread_create");
  }

  pthread_t t_uspd_id[NB_TRAINS];

  for (int i = 0; i < NB_TRAINS; i++) {
    CHECK_T(pthread_create(&t_uspd_id[i], NULL, (pf_t) thread_uspeed, (void *) (long) i), "pthread_create");
  }

  // Join threads
  for (int i = 0; i < NB_TRAINS; i++) {
    CHECK_T(pthread_join(t_ctrl_id[i], NULL), "pthread_join");
  }

  for (int i = 0; i < NB_TRAINS; i++) {
    CHECK_T(pthread_join(t_uspd_id[i], NULL), "pthread_join");
  }

  // Clean
  free(train_speed);
  free(section_locking);
  free(thread_control);
}

