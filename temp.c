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
  char data;
  strcpy(&data, "1");
  printf("%d\n", strlen(&data));
}
