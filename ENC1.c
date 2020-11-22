#include "lib.h"

int receive_from_p1(int, char*, int, char*, int, int);
int resend_message(int, char*, int);
int wait_confirmation_from_chan(int, int*);
int receive_from_chan(int, char*, int);
int confirm_to_chan(int, int*, int);
int send_to_p1(int, char*, int);

char input_from_p1[256];
char input_from_chan[256];
int main(int argc, char **argv) {
  int sem_p1_id = semget((key_t)1111, 1, 0666);

  int shm_from_p1 = shmget((key_t)2211, 256, 0666);

  char* data_from_p1 = (char*)shmat(shm_from_p1, NULL, 0);
  if(data_from_p1 == (char*)(-1)){
    printf("**Error connecting with Shared Memory- From p1**\n");
    exit(1);
  }

  int sem_second_ready = semget((key_t)5511, 1, 0666);

  int shm_id_receive = shmget((key_t)3311, 256, 0666 | IPC_CREAT);

  char* data_to_p1 = (char*)shmat(shm_id_receive, NULL, 0);
  if(data_to_p1 == (char*)(-1)){
    printf("**Error Connecting With Shared Memory- data to p1**\n");
    exit(1);
  }else{
    strcpy(data_to_p1, ""); // no need for sem cause process aint started yet
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

  int shm_confirm_chan = shmget((key_t)4412, sizeof(int), 0666 | IPC_CREAT);

  int* data_confirm_chan = (int*)shmat(shm_confirm_chan, NULL, 0);
  if(data_confirm_chan == (int*)(-1)){
    printf("**Error connecting with Shared Memory- Confirm p1->p2 chan**\n");
    exit(1);
  }

  int sem_flag_confirm_chan_re = semget((key_t)9912, 1, 0666 | IPC_CREAT);
  semctl(sem_flag_confirm_chan_re, 0, SETVAL, 0);

  int sem_flag_chan = semget((key_t)5512, 1, 0666 | IPC_CREAT);
  semctl(sem_flag_chan, 0, SETVAL, 0);
  sem_up(sem_flag_chan);


  int sem_receive_chan = semget((key_t)6612, 1, 0666 | IPC_CREAT);
  semctl(sem_receive_chan, 0, SETVAL, 1);

  int shm_receive_chan = shmget((key_t)7712, 256, 0666 | IPC_CREAT);

  char* data_receive_chan = (char*)shmat(shm_receive_chan, NULL, 0);
  if(data_receive_chan == (char*)(-1)){
    printf("**Error connecting with shared memory- receive chan");
    exit(1);
  }

  int sem_flag_chan_re = semget((key_t)11112, 1, 0666 | IPC_CREAT);
  semctl(sem_flag_chan_re, 0, SETVAL, 1);
  sem_down(sem_flag_chan_re);


  int sem_flag = semget((key_t)22211, 1, 0666);


	int step = 0;
  while(1){
    switch (step) {
      case 0: //receive from p1
        step = receive_from_p1(sem_p1_id, data_from_p1, sem_send_chan, data_send_chan, sem_flag_chan, sem_flag);
        continue;
      case 1: //retrive confirmation
        step = wait_confirmation_from_chan(sem_confirm_chan, data_confirm_chan);
        if(strcmp(input_from_p1, "TERM\n") == 0){
          break;
        }
        continue;
      case 2: //retrive from chan
        step = receive_from_chan(sem_receive_chan, data_receive_chan, sem_flag_chan_re);
        continue;
      case 3: //send confirmation
        step = confirm_to_chan(sem_confirm_chan, data_confirm_chan, sem_flag_confirm_chan_re);
        continue;
      case 4://send to p1
        step = send_to_p1(sem_p1_id, data_to_p1, sem_second_ready);
        if(strcmp(input_from_chan, "TERM\n") ==0){
          break;
        }
        continue;
      case 55://resend option
        step = resend_message(sem_send_chan, data_send_chan, sem_flag_chan);
        continue;
      case 66://from case2 i retrived falty need to resend

        continue;
    }
    break;
  }
  shmdt(data_to_p1);
  shmctl(shm_id_receive, IPC_RMID, NULL);

  semctl(sem_send_chan, 0, IPC_RMID, 0);

  shmdt(data_send_chan);
  shmctl(shm_send_chan, IPC_RMID, NULL);

  semctl(sem_confirm_chan, 0, IPC_RMID, 0);

  shmdt(data_confirm_chan);
  shmctl(shm_confirm_chan, IPC_RMID, NULL);

  semctl(sem_flag_confirm_chan_re, 0, IPC_RMID, 0);

  semctl(sem_flag_chan, 0, IPC_RMID, 0);

  semctl(sem_receive_chan, 0, IPC_RMID, 0);

  shmdt(data_receive_chan);
  shmctl(shm_receive_chan, IPC_RMID, NULL);

  semctl(sem_flag_chan_re, 0, IPC_RMID, 0);
  return 0;
}


int receive_from_p1(int sem_p1, char* data_p1, int sem_chan, char* data_chan, int flag, int flag_p1){
  sem_down(flag_p1);
  sem_down(sem_chan);
  sem_up(flag);
  sem_down(sem_p1);
      strcpy(input_from_p1, data_p1);
      strcpy(data_chan, data_p1);
      strcpy(data_p1, "WRONG1");
    sem_up(sem_chan);
  sem_up(sem_p1);
  return 1;
}

int resend_message(int sem_id, char* data, int flag){
  sem_down(sem_id);
  sem_up(flag);
    strcpy(data, input_from_p1);
  sem_up(sem_id);
  return 1;
}

int wait_confirmation_from_chan(int sem_chan, int* confirm_chan){
  sem_down(sem_chan);
  if(*confirm_chan == 1){
    sem_up(sem_chan);
    return 55;
  }
  sem_up(sem_chan);
  return 2;
}

int receive_from_chan(int sem_chan, char* data_from_chan, int flag){
  sem_down(flag);
  sem_down(sem_chan);
  strcpy(input_from_chan, data_from_chan);
  strcpy(data_from_chan, "WRONG1");
  sem_up(sem_chan);
  return 3;
}

int confirm_to_chan(int sem_chan, int* data_chan, int flag){
  // if all ok send 15 and return 4 else return 66 and return another
  sem_down(sem_chan);
  sem_up(flag);
    *data_chan = 15;
  sem_up(sem_chan);
  return 4;
}

int send_to_p1(int sem_p1, char* data, int sem_flag){
  sem_down(sem_p1);
  sem_up(sem_flag);
    strcpy(data, input_from_chan);
  sem_up(sem_p1);
  return 0;
}
