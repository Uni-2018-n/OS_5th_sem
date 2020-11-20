#include "lib.h"
union semun {                   /* Used in calls to semctl() */
    int                 val;
    struct semid_ds *   buf;
    unsigned short *    array;
};

int main(int argc, char **argv) {
	/* code */
  // printf("Client ID: %ld\nENC1\n", (long)getpid());
  int sem_id =semget((key_t)1121, 1, 0666);
  int shm_id = shmget((key_t)2221, 256, 0666);

  char* sw_user = (char*) shmat(shm_id, NULL, 0);
  if(sw_user == (char*)(-1)){
    printf("Error with sw_USER\n");
    exit(1);
  }

  char* data = (char*)malloc(256*sizeof(char));

  while(1){
    sem_down(sem_id);

    strcpy(data, sw_user);
    

    sem_up(sem_id);

    sleep(5);

    sem_down(sem_id);

    strcpy(sw_user, "**OK**");

    sem_up(sem_id);


  }
  shmdt(sw_user);
  exit(0);
}
