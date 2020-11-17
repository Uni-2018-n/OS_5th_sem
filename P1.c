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
  printf("Client ID: %ld\nP1\n", (long)getpid());

  int sem_id = semget((key_t)1111, 1, 0666 | IPC_CREAT);
  semctl(sem_id, 0, SETVAL, 1);

  int shm_id = shmget((key_t)2211, 256, 0666 | IPC_CREAT);

  // struct sembuf t={0,1,0};  //up
  // semop(sem_id, &t, 1);

  // struct sembuf t={0,-1,0};  //down
  // semop(sem_id, &t, 1);

  int pid = fork();
  if (pid < 0){
    printf("P1, error fork()");
  }else if(pid == 0){
    execvp("./ENC1", NULL);
  }else{
    while(1){
      {struct sembuf t={0,-1,0};  //down
      semop(sem_id, &t, 1);}

        //CRITICAL SECTION /*
      char input[256];
      scanf("%s\n", input);
      char *data = shmat(shm_id, NULL, 0);
      strcpy(data, input);

        //CRITICAL SECTION */
      {struct sembuf t={0,1,0};  //up
      semop(sem_id, &t, 1);}
    }

  }

  exit(0);
}
