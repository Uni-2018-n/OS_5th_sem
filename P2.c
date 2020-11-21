#include "lib.h"

int send_message(int, char*);
int receive_message(int, char*, int);
int send_confirmation(int, int*);

int main(int argc, char **argv) {
  printf("**P2  Client ID: %ld\n", (long)getpid());

  int sem_id = semget((key_t)1121, 1, 0666 | IPC_CREAT);
  semctl(sem_id, 0, SETVAL, 1);

  int shm_id_send = shmget((key_t)2221, 256, 0666 | IPC_CREAT);

  char* data_send = (char*)shmat(shm_id_send, NULL, 0);
  if(data_send == (char*)(-1)){
    printf("**Error Connecting With Shared Memory- Send**\n");
    exit(1);
  }else{
    strcpy(data_send, ""); // no need for sem cause process aint started yet
  }

  int shm_id_receive = shmget((key_t)3321, 256, 0666 | IPC_CREAT);

  char* data_receive = (char*)shmat(shm_id_receive, NULL, 0);
  if(data_receive == (char*)(-1)){
    printf("**Error Connecting With Shared Memory- Receive**\n");
    exit(1);
  }else{
    strcpy(data_receive, ""); // no need for sem cause process aint started yet
  }

  int shm_id_confirm = shmget((key_t)4421, 4, 0666 | IPC_CREAT);

  int* data_confirm = (int*)shmat(shm_id_confirm, NULL, 0);
  if(data_confirm == (int*)(-1)){
    printf("**Error Connecting With Shared Memory- Confirmation**\n");
    exit(1);
  }else{
    *data_confirm = 0; // no need for sem cause process aint started yet
  }

    int sem_second_ready = semget((key_t)5521, 1, 0666 | IPC_CREAT);
    semctl(sem_second_ready, 0, SETVAL, 0);


  int pid = fork();
  if(pid < 0){
    printf("**PID havent been created**\n");
    exit(1);
  }else if(pid == 0){
    char* arg[1];
    arg[0] = NULL;
    execvp("./ENC2", arg);
  }

  int step = 1;
  while(1){
    switch (step) {
      case 0:
      step = send_message(sem_id, data_send);
      break;
      case 1:
      step = receive_message(sem_id, data_receive, sem_second_ready);
      break;
    }
  }
}

int send_message(int sem_id, char* data){
  char input[256];
  sem_down(sem_id);

  printf("Give input from P2:");
  scanf("%s", input);
  strcpy(data, input);

  sem_up(sem_id);
  return 1;
}

int receive_message(int sem_id, char* data, int sem_second){
  printf("P1 respond: ");
  fflush(stdout);
  sem_down(sem_second);
  sem_down(sem_id);
  printf("%s\n", data);
  sem_up(sem_id);
  return 0;
}
