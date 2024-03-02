#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/mman.h>

#include "train_control.h"

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

// Missing functions
int get_section_id();
int get_next_section_id();
int lock_section(int section_id);

void signal_thread() {
    pthread_mutex_lock(&mutex);
    // Change the condition
    // ...
    pthread_cond_signal(&cond); // Wake up one waiting thread
    // or pthread_cond_broadcast(&cond); // To wake up all waiting threads if applicable
    pthread_mutex_unlock(&mutex);
}

void* thread_train(int train_id) {
  /* Déroulement du thread 
   * 1. Initialisation
   *    - Mise en place initiale du circuit avant démarragevérification
   *    - Vérification des conflit 
   *    - Positionnement initial des aiguillages
   *    - Prise initiale de ressources
   * 2. Déroulement
   *    - Arrivée dans une nouvelle section
   *    - Vérification du statut de la section suivante
   *    - Vérouillage ou non de la section suivante
   *    - Consigne de vitesse adaptée à la section suivante
   *    - Attente éventuelle
   * 3. Terminaison
   *    - Arrêt des trains
   *    - Libération des ressources
   *    - Nettoyage de la mémoire, arrêt des threads...
  */

  while(1) {
    pthread_mutex_lock(&mutex);
    while (/* condition not met */0) {
      pthread_cond_wait(&cond, &mutex);
    }
    pthread_mutex_unlock(&mutex);

  }

  return NULL;
}

// Initialisation common to all trains in same thread
// Launch threads for trains, Signal when entering new section





// Define structure for sections
//

/* Liste des fonctions nécessaires de l'API pour la communication
 * - get_section_id
 * - get_next_section_id
 * - lock_section
 * - is_section_switch
 * - set_next_section_switch
*/






// initialise a train
int init_train(int train_id) {
  int section = get_section_id();
  int next_section = get_next_section_id();

  if (lock_section(section) == -1) return -1;

  printf("initialisation ok for train %d\n", train_id);

  if (lock_section(next_section) == -1) return 1;

  return 0;
}

void thread_control(TrainSpeed *train_speed, int id) {

}

void thread_uspeed(TrackSections *track_sections, int id) {

}

int main() {
  for (int i = 0; i < NB_TRAINS; i++) {
    CHECK(init_train(i), "init_train");
  }

  // Initialize shared memomry
  // Launch thread
  // Join threads
}
