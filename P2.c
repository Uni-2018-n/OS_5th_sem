#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <ctype.h>
#include <fcntl.h>

int main(int argc, char **argv) {
	/* code */
  printf("Client ID: %ld\nP2\n", (long)getpid());
  exit(0);
}
