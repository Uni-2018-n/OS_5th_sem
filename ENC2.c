#include "lib.h"

int receive_from_p2(int, char*, int, char*, int);
int resend_message(int, char*, int);
int wait_confirmation_from_chan(int, int*);
int receive_from_chan(int, char*, int);
int confirm_to_chan(int, int*, int);
int send_to_p2(int, char*, int);

char input_from_p2[256];
char input_from_chan[256];
int main(int argc, char **argv) {
  int sem_p2_id = semget((key_t)1121, 1, 0666);

  int shm_from_p2 = shmget((key_t)2221, 256, 0666);

  char* data_from_p2 = (char*)shmat(shm_from_p2, NULL, 0);
  if(data_from_p2 == (char*)(-1)){
    printf("**Error connecting with Shared Memory- From p2**\n");
    exit(1);
  }

  int sem_second_ready = semget((key_t)5521, 1, 0666);

  int shm_id_receive = shmget((key_t)3321, 256, 0666 | IPC_CREAT);

  char* data_to_p2 = (char*)shmat(shm_id_receive, NULL, 0);
  if(data_to_p2 == (char*)(-1)){
    printf("**Error Connecting With Shared Memory- data to p2**\n");
    exit(1);
  }else{
    strcpy(data_to_p2, ""); // no need for sem cause process aint started yet
  }


  int sem_send_chan = semget((key_t)1122, 1, 0666 | IPC_CREAT);
  semctl(sem_send_chan, 0, SETVAL, 1);
  // sem_down(sem_send_chan);

  int shm_send_chan = shmget((key_t)2222, 256, 0666 | IPC_CREAT);

  char* data_send_chan = (char*)shmat(shm_send_chan, NULL, 0);
  if(data_send_chan == (char*)(-1)){
    printf("**Error connecting with Shared Memory- Send chan**\n");
    exit(1);
  }else{
    sem_down(sem_send_chan);
    strcpy(data_send_chan, "");
    sem_up(sem_send_chan);
  }


  int sem_confirm_chan = semget((key_t)3322, 1, 0666 | IPC_CREAT);
  semctl(sem_confirm_chan, 0, SETVAL, 1);

  int shm_confirm_chan = shmget((key_t)4422, sizeof(int), 0666 | IPC_CREAT);

  int* data_confirm_chan = (int*)shmat(shm_confirm_chan, NULL, 0);
  if(data_confirm_chan == (int*)(-1)){
    printf("**Error connecting with Shared Memory- Confirm p2->p1 chan**\n");
    exit(1);
  }else{
    // sem_down(sem_confirm_chan);
    // *data_confirm_chan = 120;
    // sem_up(sem_confirm_chan);
  }

  int sem_flag_confirm_chan_re = semget((key_t)9922, 1, 0666 | IPC_CREAT);
  semctl(sem_flag_confirm_chan_re, 0, SETVAL, 0);
  // sem_down(sem_flag_confirm_chan_re);

  int sem_flag_chan = semget((key_t)5522, 1, 0666 | IPC_CREAT);
  semctl(sem_flag_chan, 0, SETVAL, 0);


  int sem_receive_chan = semget((key_t)6622, 1, 0666 | IPC_CREAT);
  semctl(sem_receive_chan, 0, SETVAL, 1);

  int shm_receive_chan = shmget((key_t)7722, 256, 0666 | IPC_CREAT);

  char* data_receive_chan = (char*)shmat(shm_receive_chan, NULL, 0);
  if(data_receive_chan == (char*)(-1)){
    printf("**Error connecting with shared memory- receive chan");
    exit(1);
  }

  int sem_flag_chan_re = semget((key_t)11122, 1, 0666 | IPC_CREAT);
  semctl(sem_flag_chan_re, 0, SETVAL, 1);
  sem_down(sem_flag_chan_re);





	int step = 2;
  while(1){
    switch (step) {
      case 0: //receive from p2
        step = receive_from_p2(sem_p2_id, data_from_p2, sem_send_chan, data_send_chan, sem_flag_chan);
        break;
      case 1: //retrive confirmation
        step = wait_confirmation_from_chan(sem_confirm_chan, data_confirm_chan);
        break;
      case 2: //retrive from chan
        step = receive_from_chan(sem_receive_chan, data_receive_chan, sem_flag_chan_re);
        break;
      case 3: //send confirmation
        step = confirm_to_chan(sem_confirm_chan, data_confirm_chan, sem_flag_confirm_chan_re);
        break;
      case 4://send to p2
        step = send_to_p2(sem_p2_id, data_to_p2, sem_second_ready);
        break;
      case 55://resend option
        step = resend_message(sem_send_chan, data_send_chan, sem_flag_chan);
        break;
      case 66://from case2 i retrived falty need to resend
        break;

    }
  }
}


int receive_from_p2(int sem_p2, char* data_p2, int sem_chan, char* data_chan, int flag){
  sem_down(sem_p2);
    sem_down(sem_chan);
    sem_up(flag);
      strcpy(input_from_p2, data_p2);
      strcpy(data_chan, data_p2);
    sem_up(sem_chan);
  sem_up(sem_p2);
  return 1;
}

int resend_message(int sem_id, char* data, int flag){
  sem_down(sem_id);
  sem_up(flag);
    strcpy(data, input_from_p2);
  sem_up(sem_id);
  return 1;
}

int wait_confirmation_from_chan(int sem_chan, int* confirm_chan){
  // printf("\t\t**Waiting for confirmation**\n");
  sem_down(sem_chan);
  // *confirm_p2 = *confirm_chan;
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
  // printf("got message\n");
  strcpy(input_from_chan, data_from_chan);
  sem_up(sem_chan);
  return 3;
}

int confirm_to_chan(int sem_chan, int* data_chan, int flag){
  // if all ok send 15 and return 4 else return 66 and return another
  sem_down(sem_chan);
  sem_up(flag);
  printf("im here\n");
    *data_chan = 15;
  sem_up(sem_chan);
  return 4;
}

int send_to_p2(int sem_p2, char* data, int sem_flag){
  sem_down(sem_p2);
  sem_up(sem_flag);

  strcpy(data, input_from_chan);

  sem_up(sem_p2);
  return 0;
}
