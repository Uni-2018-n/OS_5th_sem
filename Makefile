SOURCE = P1 ENC1 CHAN ENC2 P2
ITEM = #complied file name here!

compile :
	gcc -o P1 P1.c lib.c -lcrypto
	gcc -o ENC1 ENC1.c lib.c -lcrypto
	gcc -o CHAN CHAN.c lib.c -lcrypto
	gcc -o ENC2 ENC2.c lib.c -lcrypto
	gcc -o P2 P2.c lib.c -lcrypto

clean :
	-rm $(SOURCE) *.o
