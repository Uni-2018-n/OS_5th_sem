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
  printf("Client ID: %ld\nENC1\n", (long)getpid());

  int sem_id =semget((key_t)11, 1, 0666 | IPC_CREAT);

  while(1){
    {struct sembuf t={0,-1,0};  //down
    semop(sem_id, &t, 1);}
      // CRITICAL SECTION /*


      // CRITICAL SECTION */
    {struct sembuf t={0,1,0};  //up
    semop(sem_id, &t, 1);}
  }

  exit(0);
}
