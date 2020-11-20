#include "lib.h"

int main(int argc, char **argv) {
	/* code */
  int confirmation = 1;
  printf("Client ID: %ld\nP1\n", (long)getpid());

  int sem_id = semget((key_t)1121, 1, 0666 | IPC_CREAT);
  semctl(sem_id, 0, SETVAL, 1);

  int shm_id = shmget((key_t)2221, 256, 0666 | IPC_CREAT);

  char *data = (char*)shmat(shm_id, NULL, 0);
  if(data == (char*)(-1)){
    printf("P2 ERROR\n");
  }else{
    strcpy(data, "");
  }
  int pid = fork();
  if (pid < 0){
    printf("P2, error fork()");
  }else if(pid == 0){
    char* arg[1];
    arg[1] = NULL;
    execvp("./ENC2", arg);
  }else{
    while(1){
      if(confirmation){
        sem_down(sem_id);

        char input[256];
        printf("give text: ");
        scanf("%s", input);
        strcpy(data, input);

        sem_up(sem_id);
        //wait for OK
      }
      sleep(2);

      sem_down(sem_id);
      // printf("%s\n", data);
      if(strcmp(data, "**OK**") == 0){
        printf("      **message sent!**\n");
        confirmation= 1;
      }else{
        printf("**waiting for confirmation**\n");
        confirmation=0;
      }

      sem_up(sem_id);




    }
    int status;
    waitpid(pid, &status, 0);
    shmdt(data);
    semctl(sem_id, 0, IPC_RMID, 0);
    shmctl(shm_id, IPC_RMID, NULL);

  }

  exit(0);
}
