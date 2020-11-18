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

  int sem_id_one = semget((key_t)1112, 1, 0666 | IPC_CREAT);
  int shm_id_one = shmget((key_t)2212, 1, 0666 | IPC_CREAT);
  char *from_enc1;

  while(1){

    {struct sembuf t={0, -1, 0};
    semop(sem_id_one, &t, 1);}

    from_enc1 = shmat(shm_id_one, NULL, 0);
    printf("%s\n", from_enc1);


    {struct sembuf t={0, 1, 0};
    semop(sem_id_one, &t, 1);}
  }
}
