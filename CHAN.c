#include "lib.h"


int receive_from_p1();

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


  int step=0;
  while(1){
    switch(step){
      case 0: //receive from enc1

      break;

    }
  }
}

int receive_from_enc1(){
  
}
