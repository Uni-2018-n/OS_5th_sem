#include "lib.h"

int receive_from_p1(int, int, char*, int, char*);
int return_confirmation(int, int*, int, int*);

int main(int argc, char **argv) {
  int sem_p1_id = semget((key_t)1111, 1, 0666);

  int shm_from_p1 = shmget((key_t)2211, 256, 0666);

  char* data_from_p1 = (char*)shmat(shm_from_p1, NULL, 0);
  if(data_from_p1 == (char*)(-1)){
    printf("**Error connecting with Shared Memory- From P1**\n");
    exit(1);
  }

  int sem_confirm_p1 = semget((key_t)5511, 1, 0666);

  int shm_confirm_from_p1 = shmget((key_t)4411, 4, 0666);

  int* data_confirm_from_p1 = (int*)shmat(shm_confirm_from_p1, NULL, 0);
  if(data_confirm_from_p1 == (int*)(-1)){
    printf("**Error connecting with Shared Memory- Confirm from P1**\n");
    exit(1);
  }



  int sem_send_chan = semget((key_t)1112, 1, 0666 | IPC_CREAT);
  semctl(sem_send_chan, 0, SETVAL, 1);

  int shm_send_chan = shmget((key_t)2212, 256, 0666 | IPC_CREAT);

  char* data_send_chan = (char*)shmat(shm_send_chan, NULL, 0);
  if(data_send_chan == (char*)(-1)){
    printf("**Error connecting with Shared Memory- Send chan**\n");
    exit(1);
  }else{
    sem_down(sem_send_chan);
    strcpy(data_send_chan, "");
    sem_up(sem_send_chan);
  }


  int sem_confirm_chan = semget((key_t)3312, 1, 0666 | IPC_CREAT);
  semctl(sem_confirm_chan, 0, SETVAL, 1);

  int shm_confirm_chan = shmget((key_t)4412, 4, 0666 | IPC_CREAT);

  int* data_confirm_chan = (int*)shmat(shm_confirm_chan, NULL, 0);
  if(data_confirm_chan == (int*)(-1)){
    printf("**Error connecting with Shared Memory- Confirm chan**\n");
    exit(1);
  }else{
    sem_down(sem_confirm_chan);
    *data_confirm_chan = 0;
    sem_up(sem_confirm_chan);
  }







	int step = 0;
  while(1){
    switch (step) {
      case 0: //receive from p1
        step = receive_from_p1(sem_p1_id, sem_confirm_p1, data_from_p1, sem_send_chan, data_send_chan);
        break;
      case 1: //send confirmation to p1
        step = return_confirmation(sem_confirm_p1, data_confirm_from_p1, sem_confirm_chan, data_confirm_chan);
        break;
      case 2: //receive from chan

        break;
      case 3: //send confirmation to chan

        break;

    }
  }
}


int receive_from_p1(int sem_p1, int sem_p1_confirm, char* data_p1, int sem_chan, char* data_chan){
  sem_down(sem_p1);
    sem_down(sem_p1_confirm);
    sem_down(sem_chan);
      strcpy(data_chan, data_p1);
    sem_up(sem_chan);
  sem_up(sem_p1);
  return 1;
}

int return_confirmation(int sem_p1, int* confirm_p1, int sem_chan, int* confirm_chan){
  printf("\t\t**Waiting for confirmation**\n");
  sem_down(sem_chan);
  *confirm_p1 = *confirm_chan;
  if(*confirm_p1 == 1){
    sem_up(sem_chan);
    return 0;
  }
  sem_up(sem_chan);
  return 2;
}
