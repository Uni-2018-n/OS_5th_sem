#include "lib.h"

void sem_up(int sem_id){
  {struct sembuf t={0, 1, 0};
  semop(sem_id, &t, 1);}
}

void sem_down(int sem_id){
  {struct sembuf t={0, -1, 0};
  semop(sem_id, &t, 1);}
}
