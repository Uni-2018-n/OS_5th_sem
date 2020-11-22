SOURCE = P1 ENC1 CHAN ENC2 P2
ITEM = #complied file name here!

compile :
	gcc -Wall -g -o P1 P1.c lib.c
	gcc -Wall -g -o ENC1 ENC1.c lib.c
	gcc -Wall -g -o CHAN CHAN.c lib.c
	gcc -Wall -g -o ENC2 ENC2.c lib.c
	gcc -Wall -g -o P2 P2.c lib.c

clean :
	-rm $(SOURCE) *.o
