#include "lib.h"



typedef struct rv{
  int step;
  char input[256];
}returned_value;

returned_value receive_from_enc1(int, char*, int);

int main(int argc, char **argv) {

  int sem_receive_enc1 = semget((key_t)1112, 1, 0666);

  int shm_receive_enc1 = shmget((key_t)2212, 256, 0666);

  char* data_receive_enc1 = (char*)shmat(shm_receive_enc1, NULL, 0);
  if(data_receive_enc1 == (char*)(-1)){
    printf("**Error connecting with shared memory- receive enc1**\n");
    exit(1);
  }

  int sem_confirm_enc1 = semget((key_t)3312, 1, 0666);

  int shm_confirm_enc1 = shmget((key_t)4412, 4, 0666);

  int* data_confirm_enc1 = (int*)shmat(shm_confirm_enc1, NULL, 0);
  if(data_confirm_enc1 == (int*)(-1)){
    printf("**Error connecting with shared memory- Confirm enc1**\n");
    exit(1);
  }

  int sem_flag_chan = semget((key_t)5512, 1, 0666);


  int step=0;
  returned_value temp;
  char data_from_p1[256];
  while(1){
    switch(step){
      case 0: //receive from enc1
        temp= receive_from_enc1(sem_receive_enc1, data_receive_enc1, sem_flag_chan);
        step = temp.step;
        strcpy(data_from_p1, temp.input);
      break;
      case 1: //send input to enc2

      break;
      case 2: //wait for confirmation from enc2
              // des an to confiration einai swsto, an nai tote synexise alliws allakse tor return value se ena excepsion
      break;

    }
  }
}

returned_value receive_from_enc1(int sem_id, char* data, int flag){
  sem_down(flag);
  sem_down(sem_id);
    printf("from p1: %s\n", data);
    returned_value temp;
    strcpy(temp.input, data);
  sem_up(sem_id);
  temp.step= 1;
  return temp;
}
