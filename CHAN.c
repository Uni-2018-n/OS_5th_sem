#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <ctype.h>
#include <fcntl.h>
#include <string.h>

#include <sys/sem.h>

#include <sys/shm.h>

int main(int argc, char **argv) {
	/* code */
  printf("Client ID: %ld\nCHAN\n", (long)getpid());

  int sem_id_one = semget((key_t)1112, 1, 0666);
  int shm_id_one = shmget((key_t)2212, 1, 0666);

  char* from_enc1 = (char*)shmat(shm_id_one, NULL, 0);
  if(from_enc1 == (char*)(-1)){
    printf("CHAN ERROR\n");
  }

  char* from_p1;

  while(1){

    {struct sembuf t={0, -1, 0};
    semop(sem_id_one, &t, 1);}

    if(strlen(from_enc1) != 0){
      strcpy(from_p1, from_enc1);
      printf("%s\n", from_enc1);
      strcpy(from_enc1, "");
    }else{
      strcpy(from_p1, "");
    }

    {struct sembuf t={0, 1, 0};
    semop(sem_id_one, &t, 1);}
    if(strcmp(from_p1, "TERM") ==0){
      break;
    }
  }

  shmdt(from_enc1);
  // shmctl(shm_id_one, IPC_RMID, NULL);
}
