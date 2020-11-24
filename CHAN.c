#include "lib.h"
#include <time.h>
returned_value receive_from_enc1(int, info_struct*, int);
int send_to_enc2(int, info_struct*, int, info_struct*);
returned_value wait_confirmation_from_enc2(int, int*, int);
int send_confirmation_to_enc1(int, int*, int, int);

returned_value receive_from_enc2(int, info_struct*, int);
int send_to_enc1(int, info_struct*, int, info_struct*);
returned_value wait_confirmation_from_enc1(int, int*, int);
int send_confirmation_to_enc2(int, int*, int, int);

int probability;
int main(int argc, char **argv) {
  if(argc > 2){
    printf("wrong arguments, please give 0 or non arguments\n");
    return 0;
  }else if(argc == 1){
    probability =20;
  }else{
    printf("with probability: %d\n", atoi(argv[1]));
    probability = atoi(argv[1]);
  }
  srand(time(NULL));
  int sem_receive_enc1 = semget((key_t)1112, 1, 0666);

  int shm_receive_enc1 = shmget((key_t)2212, sizeof(info_struct), 0666);

  info_struct* data_receive_enc1 = (info_struct*)shmat(shm_receive_enc1, NULL, 0);
  if(data_receive_enc1 == (info_struct*)(-1)){
    printf("**Error connecting with shared memory- receive enc1**\n");
    exit(1);
  }

  int sem_confirm_enc1_re = semget((key_t)3312, 1, 0666);

  int shm_confirm_enc1_re = shmget((key_t)4412, sizeof(int), 0666);

  int* data_confirm_enc1_re = (int*)shmat(shm_confirm_enc1_re, NULL, 0);
  if(data_confirm_enc1_re == (int*)(-1)){
    printf("**Error connecting with shared memory- Confirm enc1**\n");
    exit(1);
  }

  int sem_flag_confirm_to_enc1= semget((key_t)8812, 1, 0666);

  int sem_flag_enc1 = semget((key_t)5512, 1, 0666);

  //when receive from enc2 place here

  int sem_send_enc2 = semget((key_t)6622, 1, 0666);

  int shm_send_enc2 = shmget((key_t)7722, sizeof(info_struct), 0666);

  info_struct* data_send_enc2 = (info_struct*)shmat(shm_send_enc2, NULL, 0);
  if(data_send_enc2 == (info_struct*)(-1)){
    printf("**Error connecting with shared memory- receive chan");
    exit(1);
  }

  int sem_flag_enc2_re = semget((key_t)11122, 1, 0666);


  // confirm to enc2
  int sem_flag_confirm_enc2 = semget((key_t)9922, 1, 0666);

  int sem_confirm_enc2 = semget((key_t)3322, 1, 0666);

  int shm_confirm_enc2 = shmget((key_t)4422, sizeof(int), 0666);

  int* data_confirm_enc2 = (int*)shmat(shm_confirm_enc2, NULL, 0);
  if(data_confirm_enc2 == (int*)(-1)){
    printf("**Error connecting with shared memory- Confirm enc2**\n");
    exit(1);
  }else{
    sem_down(sem_confirm_enc2);
      *data_confirm_enc2 = 77;
    sem_up(sem_confirm_enc2);
  }




  //enc2->enc1
  int sem_receive_enc2 = semget((key_t)1122, 1, 0666);

  int shm_receive_enc2 = shmget((key_t)2222, sizeof(info_struct), 0666);

  info_struct* data_receive_enc2 = (info_struct*)shmat(shm_receive_enc2, NULL, 0);
  if(data_receive_enc2 == (info_struct*)(-1)){
    printf("**Error connecting with shared memory- receive enc2**\n");
    exit(1);
  }

  int sem_flag_enc2 = semget((key_t)5522, 1, 0666);

  int sem_send_enc1 = semget((key_t)6612, 1, 0666);

  int shm_send_enc1 = shmget((key_t)7712, sizeof(info_struct), 0666);

  info_struct* data_send_enc1 = (info_struct*)shmat(shm_send_enc1, NULL, 0);
  if(data_send_enc1 == (info_struct*)(-1)){
    printf("**Error connecting with shared memory- send from enc2 to enc1**\n");
    exit(1);
  }

  int sem_flag_enc1_re = semget((key_t)11112, 1, 0666);

  int sem_flag_confirm_enc1 = semget((key_t)9912, 1, 0666);

  int sem_confirm_enc1 = semget((key_t)3312, 1, 0666);

  int shm_confirm_enc1 = shmget((key_t)4412, sizeof(int), 0666);

  int* data_confirm_enc1 = (int*)shmat(shm_confirm_enc1, NULL, 0);
  if(data_confirm_enc1 == (int*)(-1)){
    printf("**Error connecting with shared memory- Confirm enc2**\n");
    exit(1);
  }


  int sem_confirm_enc2_re = semget((key_t)3322, 1, 0666);

  int shm_confirm_enc2_re = shmget((key_t)4422, sizeof(int), 0666);

  int* data_confirm_enc2_re = (int*)shmat(shm_confirm_enc2_re, NULL, 0);
  if(data_confirm_enc2_re == (int*)(-1)){
    printf("**Error connecting with shared memory- Confirm enc1**\n");
    exit(1);
  }

  int sem_flag_confirm_to_enc2 = semget((key_t)8822, 1, 0666);


  int step=0;
  returned_value temp;
  info_struct data_from_p1;
  info_struct data_from_p2;
  int confirmation;
  while(1){
    switch(step){
      case 0: //receive from enc1
        printf("**Step 1: waiting for message from p1**\n");
        temp= receive_from_enc1(sem_receive_enc1, data_receive_enc1, sem_flag_enc1);

        step = temp.step;
        strcpy(data_from_p1.input, temp.input);
        strcpy(data_from_p1.hash, temp.hash);
      continue;
      case 1: //send input to enc2
        printf("**Step 2: sending message to enc2**\n");
        step = send_to_enc2(sem_send_enc2, data_send_enc2, sem_flag_enc2_re, &data_from_p1);
      continue;
      case 2: //wait for confirmation from enc2
        printf("**Step 3: waiting for confirmation from enc2**\n");
        temp = wait_confirmation_from_enc2(sem_confirm_enc2, data_confirm_enc2, sem_flag_confirm_enc2);
        step = temp.step;
        confirmation = temp.confirm;
      continue;
      case 3: //send confirmation to enc1
        printf("**Step 4: sending confirmation to enc1**\n");
        step = send_confirmation_to_enc1(sem_confirm_enc1_re, data_confirm_enc1_re, confirmation, sem_flag_confirm_to_enc1);
        if(strcmp(data_from_p1.input, "TERM\n") == 0){
          break;
        }
      continue;

      // enc2 -> enc1
      case 4: //receive from enc2
        printf("**Step 5: waiting for message from p2**\n");
        temp= receive_from_enc2(sem_receive_enc2, data_receive_enc2, sem_flag_enc2);
        step = temp.step;
        strcpy(data_from_p2.input, temp.input);
        strcpy(data_from_p2.hash, temp.hash);
      continue;
      case 5: //send input to enc1
        printf("**Step 6: sending message to enc1**\n");
        step = send_to_enc1(sem_send_enc1, data_send_enc1, sem_flag_enc1_re, &data_from_p2);
      continue;
      case 6: //wait for confirmation from enc1
        printf("**Step 7: waiting for confirmation from enc1**\n");
        temp = wait_confirmation_from_enc1(sem_confirm_enc1, data_confirm_enc1, sem_flag_confirm_enc1);
        step = temp.step;
        confirmation = temp.confirm;
      continue;
      case 7: //send confirmation to enc2
        printf("**Step 8: sending confirmation to enc2**\n");
        step = send_confirmation_to_enc2(sem_confirm_enc2_re, data_confirm_enc2_re, confirmation, sem_flag_confirm_to_enc2);
        if(strcmp(data_from_p2.input, "TERM\n") == 0){
          break;
        }
      continue;
    }
    break;

  }
  shmdt(data_receive_enc1);
  shmdt(data_confirm_enc1_re);
  shmdt(data_send_enc2);
  shmdt(data_confirm_enc2);
  shmdt(data_receive_enc2);
  shmdt(data_confirm_enc1);
  shmdt(data_confirm_enc2_re);
  return 0;
}

// enc1 -> enc2
returned_value receive_from_enc1(int sem_id, info_struct* data, int flag){
  sem_down(flag);
  sem_down(sem_id);
  returned_value temp;
    printf("from p1: %s\n", data->input);
    strcpy(temp.input, data->input);
    char test[MD5_DIGEST_LENGTH];
    strcpy(test, data->hash);
    strcpy(temp.hash, test);

    strcpy(data->input, "WRONG3");
    strcpy(data->hash, "WRONG3");

  sem_up(sem_id);
  temp.step= 1;
  return temp;
}

int send_to_enc2(int sem_id, info_struct* data, int flag, info_struct* input){
  sem_down(sem_id);
  sem_up(flag);
    strcpy(data->input, input->input);
    if(strcmp(data->input, "TERM\n") != 0)
    for(unsigned int i=0;i<strlen(data->input)-1;i++){
      int x= rand()%100;
      if(x <probability){
        data->input[i] = 'x';
      }
    }
    strcpy(data->hash, input->hash);

  sem_up(sem_id);
  return 2;
}

returned_value wait_confirmation_from_enc2(int sem_id, int* data, int flag){
  returned_value temp;
  sem_down(flag);
  sem_down(sem_id);
    temp.confirm = *data;
  sem_up(sem_id);
  temp.step = 3;
  return temp;
}

int send_confirmation_to_enc1(int sem_id, int* data, int confirmation, int flag){
  sem_down(sem_id);
  sem_up(flag);
  *data = confirmation;
  sem_up(sem_id);
  if(confirmation == 15){
    return 4;
  }else{
    printf("message from enc1->enc2 will get re transmited, %d\n", confirmation);
    return 0;
  }
}



// enc2 -> enc1
returned_value receive_from_enc2(int sem_id, info_struct* data, int flag){
  sem_down(flag);
  sem_down(sem_id);
    printf("from p2: %s\n", data->input);
    returned_value temp;
    strcpy(temp.input, data->input);
    strcpy(temp.hash, data->hash);

    strcpy(data->input, "WRONG4");
    strcpy(data->hash, "WRONG4");

  sem_up(sem_id);
  temp.step= 5;
  return temp;
}

int send_to_enc1(int sem_id, info_struct* data, int flag, info_struct* input){
  sem_down(sem_id);
  sem_up(flag);
    strcpy(data->input, input->input);
    if(strcmp(data->input, "TERM\n") != 0)
    for(unsigned int i=0;i<strlen(data->input)-1;i++){
      int x= rand()%100;
      if(x <probability){
        data->input[i] = 'x';
      }
    }
    strcpy(data->hash, input->hash);
  sem_up(sem_id);
  return 6;
}

returned_value wait_confirmation_from_enc1(int sem_id, int* data, int flag){
  returned_value temp;
  sem_down(flag);
  sem_down(sem_id);
    temp.confirm = *data;
  sem_up(sem_id);
  temp.step = 7;
  return temp;
}

int send_confirmation_to_enc2(int sem_id, int* data, int confirmation, int flag){
  sem_down(sem_id);
  sem_up(flag);
  *data = confirmation;
  sem_up(sem_id);
  if(confirmation == 15){
    return 0;
  }else{
    printf("message from enc2->enc1 will get re transmited\n");
    return 4;
  }
}
