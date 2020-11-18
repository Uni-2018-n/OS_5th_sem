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
int main(int argc, char **argv) {
  int shm_id_one = shmget((key_t)2212, 1, 0666 | IPC_CREAT);
	strcpy(shmat(shm_id_one, NULL, 0), "\0");
}
