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

int main(int argc, char **argv) {
	/* code */
  printf("Client ID: %ld\nP1\n", (long)getpid());

  int sem_id = semget((key_t)11, 1, 0666 | IPC_CREAT);
  semctl(sem_id, 0, SETVAL, 0);

  // struct sembuf t={0,1,0};  //up
  // semop(sem_id, &t, 1);

  // struct sembuf t={0,-1,0};  //down
  // semop(sem_id, &t, 1);

  int pid = fork();
  if (pid < 0){
    printf("P1, error fork()");
  }else if(pid == 0){
    execvp("./ENC2", NULL);
  }else{
    while(1){
      char input[256];
      scanf("%s\n", input);
    }
  }

  exit(0);
}
