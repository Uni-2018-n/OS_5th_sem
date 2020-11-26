#include "lib.h"

int receive_from_p(int, char*, int);
int send_to_chan(int, info_struct*, int);
int wait_confirmation_from_chan(int, int*, int);
int receive_from_chan(int, info_struct*, int);
int confirm_to_chan(int, int*, int);
int send_to_p(int, char*, int);

char input_from_p[256];//used global variables so i dont have to pass them as arguments
info_struct input_from_chan;
int main(int argc, char **argv) {
  //connecting with semaphores, shared memory and creating them if needed
  // same code for key_t labels(chack CHAN.c)
  int sem_p_id = semget((key_t)1111, 1, 0666);
  if(sem_p_id < 0){
    printf("Error connecting: sem_p_id returned -1**\n");
    exit(1);
  }

  int shm_from_p = shmget((key_t)2211, 256, 0666);

  char* data_from_p = (char*)shmat(shm_from_p, NULL, 0);
  if(data_from_p == (char*)(-1)){
    printf("**Error connecting with Shared Memory- From p**\n");
    exit(1);
  }

  int sem_second_ready = semget((key_t)5511, 1, 0666);
  if(sem_second_ready < 0){
    printf("Error connecting: sem_second_ready returned -1**\n");
    exit(1);
  }

  int shm_id_receive = shmget((key_t)3311, 256, 0666 | IPC_CREAT);

  char* data_to_p = (char*)shmat(shm_id_receive, NULL, 0);
  if(data_to_p == (char*)(-1)){
    printf("**Error Connecting With Shared Memory- data to p**\n");
    exit(1);
  }


  int sem_send_chan = semget((key_t)1112, 1, 0666 | IPC_CREAT);
  if(sem_send_chan < 0){
    printf("Error connecting: sem_send_chan returned -1**\n");
    exit(1);
  }
  semctl(sem_send_chan, 0, SETVAL, 1);

  int shm_send_chan = shmget((key_t)2212, sizeof(info_struct), 0666 | IPC_CREAT);

  info_struct* data_send_chan = (info_struct*)shmat(shm_send_chan, NULL, 0);
  if(data_send_chan == (info_struct*)(-1)){
    printf("**Error connecting with Shared Memory- Send chan**\n");
    exit(1);
  }

  int sem_confirm_chan = semget((key_t)3312, 1, 0666 | IPC_CREAT);
  if(sem_confirm_chan < 0){
    printf("Error connecting: sem_confirm_chan returned -1**\n");
    exit(1);
  }
  semctl(sem_confirm_chan, 0, SETVAL, 1);

  int shm_confirm_chan = shmget((key_t)4412, sizeof(int), 0666 | IPC_CREAT);

  int* data_confirm_chan = (int*)shmat(shm_confirm_chan, NULL, 0);
  if(data_confirm_chan == (int*)(-1)){
    printf("**Error connecting with Shared Memory- Confirm p1->p2 chan**\n");
    exit(1);
  }

  int sem_flag_confirm_from_chan = semget((key_t)8812, 1, 0666 | IPC_CREAT);
  if(sem_flag_confirm_from_chan < 0){
    printf("Error connecting: sem_flag_confirm_from_chan returned -1**\n");
    exit(1);
  }
  semctl(sem_flag_confirm_from_chan, 0 , SETVAL, 0);

  int sem_flag_confirm_chan_re = semget((key_t)9912, 1, 0666 | IPC_CREAT);
  if(sem_flag_confirm_chan_re < 0){
    printf("Error connecting: sem_flag_confirm_chan_re returned -1**\n");
    exit(1);
  }
  semctl(sem_flag_confirm_chan_re, 0, SETVAL, 0);

  int sem_flag_chan = semget((key_t)5512, 1, 0666 | IPC_CREAT);
  if(sem_flag_chan < 0){
    printf("Error connecting: sem_flag_chan returned -1**\n");
    exit(1);
  }
  semctl(sem_flag_chan, 0, SETVAL, 0);

  int sem_receive_chan = semget((key_t)6612, 1, 0666 | IPC_CREAT);
  if(sem_receive_chan < 0){
    printf("Error connecting: sem_receive_chan returned -1**\n");
    exit(1);
  }
  semctl(sem_receive_chan, 0, SETVAL, 1);

  int shm_receive_chan = shmget((key_t)7712, sizeof(info_struct), 0666 | IPC_CREAT);

  info_struct* data_receive_chan = (info_struct*)shmat(shm_receive_chan, NULL, 0);
  if(data_receive_chan == (info_struct*)(-1)){
    printf("**Error connecting with shared memory- receive chan");
    exit(1);
  }

  int sem_flag_chan_re = semget((key_t)11112, 1, 0666 | IPC_CREAT);
  if(sem_flag_chan_re < 0){
    printf("Error connecting: sem_flag_chan_re returned -1**\n");
    exit(1);
  }
  semctl(sem_flag_chan_re, 0, SETVAL, 1);
  sem_down(sem_flag_chan_re);

  int sem_flag = semget((key_t)22211, 1, 0666);
  if(sem_flag < 0){
    printf("Error connecting: sem_flag returned -1**\n");
    exit(1);
  }


  //same step, switch-case, loop logic used as CHAN.c
	int step = 0;
  while(1){
    switch (step) {
      case 0: //receive from p
        step = receive_from_p(sem_p_id, data_from_p, sem_flag);
        continue;
      case 1://send to chan
        step = send_to_chan(sem_send_chan, data_send_chan, sem_flag_chan);
        continue;
      case 2: //retrive confirmation from chan
        step = wait_confirmation_from_chan(sem_confirm_chan, data_confirm_chan, sem_flag_confirm_from_chan);
        if(strcmp(input_from_p, "TERM\n") == 0){
          break;
        }
        continue;
      case 3: //retrive from chan
        step = receive_from_chan(sem_receive_chan, data_receive_chan, sem_flag_chan_re);
        continue;
      case 4: //send confirmation to chan
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
  shmdt(data_to_p);//remove all created semaphores
  shmctl(shm_id_receive, IPC_RMID, NULL);//detach from any used shared memory
                                        //and if it was created from this process then RMID it
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

//used same sem-flag logic as CHAN.c
int receive_from_p(int sem_p, char* data_p, int flag){
  sem_down(flag);
  sem_down(sem_p);
      strcpy(input_from_p, data_p);
  sem_up(sem_p);
  return 1;
}

int send_to_chan(int sem_chan, info_struct* data_chan, int flag){
  char hash[MD5_DIGEST_LENGTH];//create a variable for the hash
  MD5(input_from_p, sizeof(input_from_p), hash); //hash the input given by P
  sem_down(sem_chan);
  sem_up(flag);
    strcpy(data_chan->input, input_from_p);
    strcpy(data_chan->hash, hash);//save it to CHAN's shared memory for sending data
  sem_up(sem_chan);
  return 2;
}

int wait_confirmation_from_chan(int sem_chan, int* confirm_chan, int flag){
  sem_down(flag);
  sem_down(sem_chan);
    if(*confirm_chan == 1){//if received 1 then we need to retransmit
      sem_up(sem_chan);
      return 1;//return the correct value
    }
  sem_up(sem_chan);//if not continue as normal
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
  char hash[MD5_DIGEST_LENGTH];
  MD5(input_from_chan.input, sizeof(input_from_chan.input), hash);
  sem_down(sem_chan);
  sem_up(flag);
  if(strcmp(input_from_chan.input, "TERM\n") == 0){//if the input is TERM then we dont have to check if its correct
      *data_chan = 15;//send confirmed value as correct
      sem_up(sem_chan);
      return 5;
  }else{//if not
    if(memcmp(input_from_chan.hash, hash, MD5_DIGEST_LENGTH) == 0){//check if the 2 hash values are the same
      *data_chan = 15;//if yes the message is correct
      sem_up(sem_chan);
      return 5;
    }else{
      *data_chan = 1;//else request retrasmition
      sem_up(sem_chan);
      return 3;//go to the correct next step to receive the retrasmition
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
