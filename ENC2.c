#include "lib.h"

int receive_from_p(int, char*, int);
int send_to_chan(int, info_struct*, int);
int wait_confirmation_from_chan(int, int*, int);
int receive_from_chan(int, info_struct*, int);
int confirm_to_chan(int, int*, int);
int send_to_p(int, char*, int);

char input_from_p[256];
info_struct input_from_chan;

int main(int argc, char **argv) {
  int sem_p_id = semget((key_t)1121, 1, 0666);

  int shm_from_p = shmget((key_t)2221, 256, 0666);

  char* data_from_p = (char*)shmat(shm_from_p, NULL, 0);
  if(data_from_p == (char*)(-1)){
    printf("**Error connecting with Shared Memory- From p**\n");
    exit(1);
  }

  int sem_second_ready = semget((key_t)5521, 1, 0666);

  int shm_id_receive = shmget((key_t)3321, 256, 0666 | IPC_CREAT);

  char* data_to_p = (char*)shmat(shm_id_receive, NULL, 0);
  if(data_to_p == (char*)(-1)){
    printf("**Error Connecting With Shared Memory- data to p**\n");
    exit(1);
  }else{
    strcpy(data_to_p, ""); // no need for sem cause process aint started yet
  }


  int sem_send_chan = semget((key_t)1122, 1, 0666 | IPC_CREAT);
  semctl(sem_send_chan, 0, SETVAL, 1);

  int shm_send_chan = shmget((key_t)2222, sizeof(info_struct), 0666 | IPC_CREAT);

  info_struct* data_send_chan = (info_struct*)shmat(shm_send_chan, NULL, 0);
  if(data_send_chan == (info_struct*)(-1)){
    printf("**Error connecting with Shared Memory- Send chan**\n");
    exit(1);
  }

  int sem_confirm_chan = semget((key_t)3322, 1, 0666 | IPC_CREAT);
  semctl(sem_confirm_chan, 0, SETVAL, 1);

  int shm_confirm_chan = shmget((key_t)4422, sizeof(int), 0666 | IPC_CREAT);

  int* data_confirm_chan = (int*)shmat(shm_confirm_chan, NULL, 0);
  if(data_confirm_chan == (int*)(-1)){
    printf("**Error connecting with Shared Memory- Confirm p2->p1 chan**\n");
    exit(1);
  }

  int sem_flag_confirm_from_chan = semget((key_t)8822, 1, 0666 | IPC_CREAT);
  semctl(sem_flag_confirm_from_chan, 0 , SETVAL, 0);

  int sem_flag_confirm_chan_re = semget((key_t)9922, 1, 0666 | IPC_CREAT);
  semctl(sem_flag_confirm_chan_re, 0, SETVAL, 0);

  int sem_flag_chan = semget((key_t)5522, 1, 0666 | IPC_CREAT);
  semctl(sem_flag_chan, 0, SETVAL, 0);

  int sem_receive_chan = semget((key_t)6622, 1, 0666 | IPC_CREAT);
  semctl(sem_receive_chan, 0, SETVAL, 1);

  int shm_receive_chan = shmget((key_t)7722, sizeof(info_struct), 0666 | IPC_CREAT);

  info_struct* data_receive_chan = (info_struct*)shmat(shm_receive_chan, NULL, 0);
  if(data_receive_chan == (info_struct*)(-1)){
    printf("**Error connecting with shared memory- receive chan");
    exit(1);
  }

  int sem_flag_chan_re = semget((key_t)11122, 1, 0666 | IPC_CREAT);
  semctl(sem_flag_chan_re, 0, SETVAL, 1);
  sem_down(sem_flag_chan_re);


  int sem_flag = semget((key_t)22221, 1, 0666);


	int step = 3;
  while(1){
    switch (step) {
      case 0: //receive from p
        step = receive_from_p(sem_p_id, data_from_p, sem_flag);
        continue;
      case 1:
        step = send_to_chan(sem_send_chan, data_send_chan, sem_flag_chan);
        continue;
      case 2: //retrive confirmation
        step = wait_confirmation_from_chan(sem_confirm_chan, data_confirm_chan, sem_flag_confirm_from_chan);
        if(strcmp(input_from_p, "TERM\n") == 0){
          break;
        }
        continue;
      case 3: //retrive from chan
        step = receive_from_chan(sem_receive_chan, data_receive_chan, sem_flag_chan_re);
        continue;
      case 4: //send confirmation
        step = confirm_to_chan(sem_confirm_chan, data_confirm_chan, sem_flag_confirm_chan_re);
        continue;
      case 5://send to p
        step = send_to_p(sem_p_id, data_to_p, sem_second_ready);
        if(strcmp(input_from_chan.input, "TERM\n") ==0){
          break;
        }
        continue;
    }
    break;
  }
  shmdt(data_to_p);
  shmctl(shm_id_receive, IPC_RMID, NULL);

  semctl(sem_send_chan, 0, IPC_RMID, 0);

  shmdt(data_send_chan);
  shmctl(shm_send_chan, IPC_RMID, NULL);

  semctl(sem_confirm_chan, 0, IPC_RMID, 0);

  shmdt(data_confirm_chan);
  shmctl(shm_confirm_chan, IPC_RMID, NULL);

  semctl(sem_flag_confirm_from_chan, 0, IPC_RMID, 0);

  semctl(sem_flag_confirm_chan_re, 0, IPC_RMID, 0);

  semctl(sem_flag_chan, 0, IPC_RMID, 0);

  semctl(sem_receive_chan, 0, IPC_RMID, 0);

  shmdt(data_receive_chan);
  shmctl(shm_receive_chan, IPC_RMID, NULL);

  semctl(sem_flag_chan_re, 0, IPC_RMID, 0);
  return 0;
}


int receive_from_p(int sem_p, char* data_p, int flag){
  sem_down(flag);
  sem_down(sem_p);
      strcpy(input_from_p, data_p);
  sem_up(sem_p);
  return 1;
}

int send_to_chan(int sem_chan, info_struct* data_chan, int flag){
  sem_down(sem_chan);
  sem_up(flag);
    strcpy(data_chan->input, input_from_p);
    char hash[MD5_DIGEST_LENGTH];
    MD5(input_from_p, sizeof(input_from_p), hash);
    strcpy(data_chan->hash, hash);
  sem_up(sem_chan);
  return 2;
}

int wait_confirmation_from_chan(int sem_chan, int* confirm_chan, int flag){
  sem_down(flag);
  sem_down(sem_chan);
    if(*confirm_chan == 1){
      sem_up(sem_chan);
      return 1;
    }
  sem_up(sem_chan);
  return 3;
}

int receive_from_chan(int sem_chan, info_struct* data_from_chan, int flag){
  sem_down(flag);
  sem_down(sem_chan);
    strcpy(input_from_chan.input, data_from_chan->input);
    strcpy(input_from_chan.hash, data_from_chan->hash);
  sem_up(sem_chan);
  return 4;
}

int confirm_to_chan(int sem_chan, int* data_chan, int flag){
  // if all ok send 15 and return 4 else return 66 and return another
  sem_down(sem_chan);
  sem_up(flag);
  char hash[MD5_DIGEST_LENGTH];
  MD5(input_from_chan.input, sizeof(input_from_chan.input), hash);
  if(strcmp(input_from_chan.input, "TERM\n") == 0){
      *data_chan = 15;
      sem_up(sem_chan);
      return 5;
  }else{
    if(memcmp(input_from_chan.hash, hash, MD5_DIGEST_LENGTH) == 0){
      *data_chan = 15;
      sem_up(sem_chan);
      return 5;
    }else{
      *data_chan = 1;
      sem_up(sem_chan);
      return 3;
    }
  }
}

int send_to_p(int sem_p, char* data, int sem_flag){
  sem_down(sem_p);
  sem_up(sem_flag);
    strcpy(data, input_from_chan.input);
  sem_up(sem_p);
  return 0;
}
