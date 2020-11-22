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

typedef struct rv{
  int step;
  char input[256];
  int confirm;
}returned_value;

void sem_up(int sem_id);
void sem_down(int sem_id);
