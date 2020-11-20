/*
confirm:
0000 X
0001 RECEND
0010
0011
0100
0101
0110
0111
1000
1001
1010
1011
1100
1101
1110
1111 DATA RECEIVED
*/

#include "lib.h"

int send_message(int, char*);
int resend_message(int, char*, char*);
int wait_confirmation(int, int*);
int receive_message(int, char*);
int send_confirmation(int, int*);

char input[256];

int main(int argc, char **argv) {
  printf("**P1  Client ID: %ld\n", (long)getpid());

  int sem_id = semget((key_t)1111, 1, 0666 | IPC_CREAT);
  semctl(sem_id, 0, SETVAL, 1);

  int shm_id_send = shmget((key_t)2211, 256, 0666 | IPC_CREAT);

  char* data_send = (char*)shmat(shm_id_send, NULL, 0);
  if(data_send == (char*)(-1)){
    printf("**Error Connecting With Shared Memory- Send**\n");
    exit(1);
  }else{
    strcpy(data_send, ""); // no need for sem cause process aint started yet
  }

  int shm_id_receive = shmget((key_t)3311, 256, 0666 | IPC_CREAT);

  char* data_receive = (char*)shmat(shm_id_receive, NULL, 0);
  if(data_receive == (char*)(-1)){
    printf("**Error Connecting With Shared Memory- Receive**\n");
    exit(1);
  }else{
    strcpy(data_receive, ""); // no need for sem cause process aint started yet
  }

  int shm_id_confirm = shmget((key_t)4411, 4, 0666 | IPC_CREAT);

  int* data_confirm = (int*)shmat(shm_id_confirm, NULL, 0);
  if(data_confirm == (int*)(-1)){
    printf("**Error Connecting With Shared Memory- Confirmation**\n");
    exit(1);
  }else{
    *data_confirm = 0; // no need for sem cause process aint started yet
  }

  int sem_id_confirm = semget((key_t)5511, 1, 0666 | IPC_CREAT);
  semctl(sem_id, 0, SETVAL, 1);

  int pid = fork();
  if(pid < 0){
    printf("**PID havent been created**\n");
    exit(1);
  }else if(pid == 0){
    char* arg[1];
    arg[0] = NULL;
    execvp("./ENC1", arg);
  }

  int step = 0;
  while(1){
    switch (step) {
      case 0:
      step = send_message(sem_id, data_send);
      break;
      case 1:
      step = wait_confirmation(sem_id_confirm, data_confirm);
      if(step == 1){
        step = resend_message(sem_id, data_send, input);
      }
      break;
      case 2:
      step = receive_message(sem_id, data_receive);
      break;
      case 3:
      step = send_confirmation(sem_id_confirm, data_confirm);
      break;
    }
  }
}

int send_message(int sem_id, char* data){
  sem_down(sem_id);

  printf("Give input from P1:");
  scanf("%s", input);
  strcpy(data, input);

  sem_up(sem_id);
  return 1;
}

int resend_message(int sem_id, char* data, char* input){
  sem_down(sem_id);

  strcpy(data, input);

  sem_up(sem_id);
  return 1;
}

int wait_confirmation(int sem_id, int* data){
  sem_up(sem_id);
  while(1){
    switch (*data) {
      case 15:
        printf("\t\t**Message Sent!**\n");
        sem_down(sem_id);
        return 2;
        break;
      case 1:
        printf("\t\t**Message Failed, Resending...**\n");
        sem_down(sem_id);
        return 1;
        break;
    }
}
}

int receive_message(int sem_id, char* data){

}

int send_confirmation(int sem_id, int* data){

}
