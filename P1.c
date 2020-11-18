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

#include <sys/wait.h>


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
      printf("give text from p1: ");
      scanf("%s", input);

      char *data = (char*)shmat(shm_id, NULL, 0);
      if(data == (char*)(-1)){
        printf("P1 ERROR\n");
      }else{
        strcpy(data, input);
      }
      shmdt(data);

        //CRITICAL SECTION */
      {struct sembuf t={0,1,0};  //up
      semop(sem_id, &t, 1);}
      if(strcmp(input, "TERM")==0){
        break;
      }
    }
    int status;
    waitpid(pid, &status, 0);
    semctl(sem_id, 0, IPC_RMID, 0);
    shmctl(shm_id, IPC_RMID, NULL);

  }

  exit(0);
}
