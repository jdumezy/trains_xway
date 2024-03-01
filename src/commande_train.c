#include <pthread.h>
#include <semaphore.h>

#include "commande_train.h"

// Initialise a train
int init_train(int train_id) {
  int section = get_section_id();
  int next_section = get_next_section_id(train_id);

  if (lock_section(section) == -1) return -1;

  printf("Initialisation OK for train %d\n", train_id);

  if (lock_section(next_section) == -1) return 1;

  return 0;
}

void thread_train(int train_id) {
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
    pthread

  }
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
