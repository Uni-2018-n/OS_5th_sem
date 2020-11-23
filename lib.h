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

#include <sys/wait.h>

#include <openssl/md5.h>


typedef struct rv{
  int step;
  char input[256];
  int confirm;
  char hash[MD5_DIGEST_LENGTH];
}returned_value;

typedef struct is{
  char input[256];
  char hash[MD5_DIGEST_LENGTH];
}info_struct;


void sem_up(int sem_id);
void sem_down(int sem_id);
