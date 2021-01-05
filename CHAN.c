#include "lib.h"
#include <time.h>
#define DEBUG
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
    printf("**Error: Wrong arguments, Please give 1 or Non**\n");
    exit(1);
  }else if(argc == 1){
    probability =2;
    printf("Default probability: 2\n");
  }else{
    probability = atoi(argv[1]);
    if(probability >99 || probability <1){
      printf("**Error: Wrong input, use values from 1 to 99**\n");
    }
  }

  srand(time(NULL));

  //Getting all the semaphores and the shared memory that the other processes use
  //(key_t) code: if its 4 numbers: first 2 are to specify the action
  //                                next one depens if we talk about the enc1 or enc2
  //                                last one is the layer(p1->enc1 is first layer, enc1->chan is second layer)
  int sem_receive_enc1 = semget((key_t)1112, 1, 0666);
  if(sem_receive_enc1 < 0){
    printf("**Error connecting: sem receive enc1 returned -1**\n"); //if you get one of these propably there is a problem with sem/shared memory or you havent open a process
    exit(1);
  }

  int shm_receive_enc1 = shmget((key_t)2212, sizeof(info_struct), 0666);

  info_struct* data_receive_enc1 = (info_struct*)shmat(shm_receive_enc1, NULL, 0);
  if(data_receive_enc1 == (info_struct*)(-1)){
    printf("**Error connecting with shared memory- receive enc1**\n");
    exit(1);
  }

  int sem_confirm_enc1_re = semget((key_t)3312, 1, 0666);
  if(sem_confirm_enc1_re < 0){
    printf("**Error connecting: sem confirm enc1 re returned -1**\n");
    exit(1);
  }

  int shm_confirm_enc1_re = shmget((key_t)4412, sizeof(int), 0666);

  int* data_confirm_enc1_re = (int*)shmat(shm_confirm_enc1_re, NULL, 0);
  if(data_confirm_enc1_re == (int*)(-1)){
    printf("**Error connecting with shared memory- Confirm enc1**\n");
    exit(1);
  }

  int sem_flag_confirm_to_enc1= semget((key_t)8812, 1, 0666);
  if(sem_flag_confirm_to_enc1 < 0){
    printf("**Error connecting: sem_flag_confirm_to_enc1 returned -1**\n");
    exit(1);
  }

  int sem_flag_enc1 = semget((key_t)5512, 1, 0666);
  if(sem_flag_enc1 < 0){
    printf("**Error connecting: sem_flag_enc1 returned -1**\n");
    exit(1);
  }

  int sem_send_enc2 = semget((key_t)6622, 1, 0666);
  if(sem_send_enc2 < 0){
    printf("**Error connecting: sem_send_enc2 returned -1**\n");
    exit(1);
  }

  int shm_send_enc2 = shmget((key_t)7722, sizeof(info_struct), 0666);

  info_struct* data_send_enc2 = (info_struct*)shmat(shm_send_enc2, NULL, 0);
  if(data_send_enc2 == (info_struct*)(-1)){
    printf("**Error connecting with shared memory- receive chan");
    exit(1);
  }

  int sem_flag_enc2_re = semget((key_t)11122, 1, 0666);
  if(sem_flag_enc2_re < 0){
    printf("**Error connecting: sem_flag_enc2_re returned -1**\n");
    exit(1);
  }

  int sem_flag_confirm_enc2 = semget((key_t)9922, 1, 0666);
  if(sem_flag_confirm_enc2 < 0){
    printf("**Error connecting: sem_flag_confirm_enc2 returned -1**\n");
    exit(1);
  }

  int sem_confirm_enc2 = semget((key_t)3322, 1, 0666);
  if(sem_confirm_enc2 < 0){
    printf("**Error connecting: sem_confirm_enc2 returned -1**\n");
    exit(1);
  }

  int shm_confirm_enc2 = shmget((key_t)4422, sizeof(int), 0666);

  int* data_confirm_enc2 = (int*)shmat(shm_confirm_enc2, NULL, 0);
  if(data_confirm_enc2 == (int*)(-1)){
    printf("**Error connecting with shared memory- Confirm enc2**\n");
    exit(1);
  }



  //enc2->enc1
  int sem_receive_enc2 = semget((key_t)1122, 1, 0666);
  if(sem_receive_enc2 < 0){
    printf("**Error connecting: sem_receive_enc2 returned -1**\n");
    exit(1);
  }

  int shm_receive_enc2 = shmget((key_t)2222, sizeof(info_struct), 0666);

  info_struct* data_receive_enc2 = (info_struct*)shmat(shm_receive_enc2, NULL, 0);
  if(data_receive_enc2 == (info_struct*)(-1)){
    printf("**Error connecting with shared memory- receive enc2**\n");
    exit(1);
  }

  int sem_flag_enc2 = semget((key_t)5522, 1, 0666);
  if(sem_flag_enc2 < 0){
    printf("**Error connecting: sem_flag_enc2 returned -1**\n");
    exit(1);
  }

  int sem_send_enc1 = semget((key_t)6612, 1, 0666);
  if(sem_send_enc1 < 0){
    printf("**Error connecting: sem_send_enc1 returned -1**\n");
    exit(1);
  }

  int shm_send_enc1 = shmget((key_t)7712, sizeof(info_struct), 0666);

  info_struct* data_send_enc1 = (info_struct*)shmat(shm_send_enc1, NULL, 0);
  if(data_send_enc1 == (info_struct*)(-1)){
    printf("**Error connecting with shared memory- send from enc2 to enc1**\n");
    exit(1);
  }

  int sem_flag_enc1_re = semget((key_t)11112, 1, 0666);
  if(sem_flag_enc1_re < 0){
    printf("**Error connecting: sem_flag_enc1_re returned -1**\n");
    exit(1);
  }

  int sem_flag_confirm_enc1 = semget((key_t)9912, 1, 0666);
  if(sem_flag_confirm_enc1 < 0){
    printf("**Error connecting: sem_flag_confirm_enc1 returned -1**\n");
    exit(1);
  }

  int sem_confirm_enc1 = semget((key_t)3312, 1, 0666);
  if(sem_confirm_enc1 < 0){
    printf("**Error connecting: sem_confirm_enc1 returned -1**\n");
    exit(1);
  }

  int shm_confirm_enc1 = shmget((key_t)4412, sizeof(int), 0666);

  int* data_confirm_enc1 = (int*)shmat(shm_confirm_enc1, NULL, 0);
  if(data_confirm_enc1 == (int*)(-1)){
    printf("**Error connecting with shared memory- Confirm enc2**\n");
    exit(1);
  }

  int sem_confirm_enc2_re = semget((key_t)3322, 1, 0666);
  if(sem_confirm_enc2_re < 0){
    printf("**Error connecting: sem_confirm_enc2_re returned -1**\n");
    exit(1);
  }

  int shm_confirm_enc2_re = shmget((key_t)4422, sizeof(int), 0666);

  int* data_confirm_enc2_re = (int*)shmat(shm_confirm_enc2_re, NULL, 0);
  if(data_confirm_enc2_re == (int*)(-1)){
    printf("**Error connecting with shared memory- Confirm enc1**\n");
    exit(1);
  }

  int sem_flag_confirm_to_enc2 = semget((key_t)8822, 1, 0666);
  if(sem_flag_confirm_to_enc2 < 0){
    printf("**Error connecting: sem_flag_confirm_to_enc2 returned -1**\n");
    exit(1);
  }



  int step=0; //step is used so we know the situation we are in
  returned_value temp; //temporary variables and variables to store data to pass to next step
  info_struct data_from_p1;
  info_struct data_from_p2;
  int confirmation;
  while(1){
    switch(step){
      case 0: //receive from enc1
        #ifdef DEBUG
        printf("\n**Step 1: waiting for message from p1**\n");
        #endif
        temp= receive_from_enc1(sem_receive_enc1, data_receive_enc1, sem_flag_enc1);
        step = temp.step;
        strcpy(data_from_p1.input, temp.input);
        strcpy(data_from_p1.hash, temp.hash);
      continue;//continue instead of break so we go to next loop instead of continue the code in this one
      case 1: //send input to enc2
        #ifdef DEBUG
        printf("**Step 2: sending message to enc2**\n");
        #endif
        step = send_to_enc2(sem_send_enc2, data_send_enc2, sem_flag_enc2_re, &data_from_p1);
      continue;
      case 2: //wait confirmation from enc2
        #ifdef DEBUG
        printf("**Step 3: waiting confirmation from enc2**\n");
        #endif
        temp = wait_confirmation_from_enc2(sem_confirm_enc2, data_confirm_enc2, sem_flag_confirm_enc2);
        step = temp.step;
        confirmation = temp.confirm;
      continue;
      case 3: //send confirmation to enc1
        #ifdef DEBUG
        printf("**Step 4: sending confirmation to enc1**\n");
        #endif
        step = send_confirmation_to_enc1(sem_confirm_enc1_re, data_confirm_enc1_re, confirmation, sem_flag_confirm_to_enc1);
        if(strcmp(data_from_p1.input, "TERM\n") == 0){ //case of termination, to finish the proccess
          printf("Terminating\n");
          break; //break so we stop the switch-case and then break the loop
        }
      continue;

      // enc2 -> enc1
      case 4: //receive from enc2
        #ifdef DEBUG
        printf("\n**Step 5: waiting for message from p2**\n");
        #endif
        temp= receive_from_enc2(sem_receive_enc2, data_receive_enc2, sem_flag_enc2);
        step = temp.step;
        strcpy(data_from_p2.input, temp.input);
        strcpy(data_from_p2.hash, temp.hash);
      continue;
      case 5: //send input to enc1
        #ifdef DEBUG
        printf("**Step 6: sending message to enc1**\n");
        #endif
        step = send_to_enc1(sem_send_enc1, data_send_enc1, sem_flag_enc1_re, &data_from_p2);
      continue;
      case 6: //wait for confirmation from enc1
        #ifdef DEBUG
        printf("**Step 7: waiting for confirmation from enc1**\n");
        #endif
        temp = wait_confirmation_from_enc1(sem_confirm_enc1, data_confirm_enc1, sem_flag_confirm_enc1);
        step = temp.step;
        confirmation = temp.confirm;
      continue;
      case 7: //send confirmation to enc2
        #ifdef DEBUG
        printf("**Step 8: sending confirmation to enc2**\n");
        #endif
        step = send_confirmation_to_enc2(sem_confirm_enc2_re, data_confirm_enc2_re, confirmation, sem_flag_confirm_to_enc2);
        if(strcmp(data_from_p2.input, "TERM\n") == 0){
          printf("Terminating\n");
          break;
        }
      continue;
    }
    break;
  }
  shmdt(data_receive_enc1);//detach from all the shared memory
  shmdt(data_confirm_enc1_re);
  shmdt(data_send_enc2);
  shmdt(data_confirm_enc2);
  shmdt(data_receive_enc2);
  shmdt(data_send_enc1);
  shmdt(data_confirm_enc1);
  shmdt(data_confirm_enc2_re);
  return 0;
}

// enc1 -> enc2
returned_value receive_from_enc1(int sem_id, info_struct* data, int flag){
  returned_value temp;
  sem_down(flag);//used for sync so this process will always get access to the sem_id always second
  sem_down(sem_id);//if data is not occupied
    strcpy(temp.input, data->input);//use the shared memory
    strcpy(temp.hash, data->hash);
  sem_up(sem_id); //stop making it occupied
  printf("from p1: %s", temp.input);//do everything else needed
  temp.step= 1;//return 1 so we can go to the next step
  return temp;
}

int send_to_enc2(int sem_id, info_struct* data, int flag, info_struct* input){
  if(strcmp(input->input, "TERM\n") != 0){//if its TERM we dont touch it cause its a "flag"
    for(unsigned int i=0;i<strlen(input->input)-1;i++){//if not we go through every letter(except last cause its \n)
      int x= rand()%100;
      if(x <probability){//if we hit a random number lower than the probability
        input->input[i] = 'a' + (rand() % 26); //replace it with a random letter so its "corrupted"
      }
    }
  }
  sem_down(sem_id);//same logic with the flags above but now we always need to use the memory first instead of second
  sem_up(flag); //so we occupie the sem_id first and then we de-occupie the flag for the other process to start waiting the sem_id
    strcpy(data->input, input->input);
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
  if(confirmation == 15){//if its 15 (in binary 1111 thats why 15)
    return 4; //its ok so no need to change the CHAN's flow
  }else{ //if not, we have to receive a retransmitted message
    #ifdef DEBUG
    printf("**Confirmation Error: Message enc1->enc2 will get retransmitted**\n");
    #endif
    return 0; //so go back to start
  }
}



// enc2 -> enc1, code is exactly the same as enc1->enc2, only next step values change
returned_value receive_from_enc2(int sem_id, info_struct* data, int flag){
  returned_value temp;
  sem_down(flag);
  sem_down(sem_id);
    strcpy(temp.input, data->input);
    strcpy(temp.hash, data->hash);
  sem_up(sem_id);
  printf("from p2: %s", temp.input);
  temp.step= 5;
  return temp;
}

int send_to_enc1(int sem_id, info_struct* data, int flag, info_struct* input){
  if(strcmp(input->input, "TERM\n") != 0){
    for(unsigned int i=0;i<strlen(input->input)-1;i++){
      int x= rand()%100;
      if(x <probability){
        input->input[i] = 'a' + (rand() % 26);
      }
    }
  }
  sem_down(sem_id);
  sem_up(flag);
    strcpy(data->input, input->input);
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
    #ifdef DEBUG
    printf("**Confirmation Error: Message enc2->enc1 will get retransmitted**\n");
    #endif
    return 4;
  }
}
