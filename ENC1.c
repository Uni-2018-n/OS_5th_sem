#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <ctype.h>
#include <fcntl.h>
#include <string.h>
#include <sys/sem.h>

#include <sys/shm.h>
union semun {                   /* Used in calls to semctl() */
    int                 val;
    struct semid_ds *   buf;
    unsigned short *    array;
};

int main(int argc, char **argv) {
	/* code */
  // printf("Client ID: %ld\nENC1\n", (long)getpid());

  int sem_id =semget((key_t)1111, 1, 0666);
  int shm_id = shmget((key_t)2211, 256, 0666);

  int sem_id_chan = semget((key_t)1112, 1, 0666 | IPC_CREAT);
  union semun temp;
	temp.val = 1;
  semctl(sem_id_chan, 0, SETVAL, temp);
  int shm_id_chan = shmget((key_t)2212, 256, 0666 | IPC_CREAT);
  if(shm_id_chan < 0){
    printf("HERE ERROR\n");
  }
  // printf("shm_id_chan: %d\n", shm_id_chan);

  char *data = (char*)shmat(shm_id, NULL, 0);
  if(data == (char*)(-1)){
    printf("ENC1 ERROR\n");
    exit(1);
  }

  char *tochan_data = (char*)shmat(shm_id_chan, NULL, 0);
  if(tochan_data == (char *)(-1)){
    printf("ENC1 ERROR_CHAN\n");
    exit(1);
  }

  // char *tochan_data;
  while(1){
    char* from_pone;
    {struct sembuf t={0,-1,0};  //down
    semop(sem_id, &t, 1);}
      // CRITICAL SECTION /*

      //reading
      strcpy(from_pone, data);


      {struct sembuf t={0, -1, 0};
      semop(sem_id_chan, &t, 1);}

      strcpy(tochan_data, from_pone);

      {struct sembuf t={0, 1, 0};
      semop(sem_id_chan, &t, 1);}


      // CRITICAL SECTION */
    {struct sembuf t={0,1,0};  //up
    semop(sem_id, &t, 1);}

    if(strcmp(from_pone, "TERM") == 0){
      break;
    }
  }
  // printf("test\n");
  semctl(sem_id_chan, 0, IPC_RMID, 0);
  shmdt(data);
  shmdt(tochan_data);
  shmctl(shm_id_chan, IPC_RMID, NULL);
  exit(0);
}
