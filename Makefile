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
	ipcrm -M 0x8a3
	ipcrm -M 0xcef
	ipcrm -M 0x113b
	ipcrm -M 0x8a4
	ipcrm -M 0x113c
	ipcrm -S 0x457
	ipcrm -S 0x1587
	ipcrm -S 0x458
	ipcrm -S 0xcf0
	ipcrm -S 0x1588
	ipcrm -S 0x19d4
	ipcrm -M 0x1e20

