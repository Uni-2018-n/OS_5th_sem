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
	ipcrm -M 0x1e20
	ipcrm -M 0x8ad
	ipcrm -M 0xcf9
	ipcrm -M 0x1145
	ipcrm -M 0x8ae
	ipcrm -M 0x1146
	ipcrm -M 0x1e2a

	ipcrm -S 0x457
	ipcrm -S 0x1587
	ipcrm -S 0x458
	ipcrm -S 0xcf0
	ipcrm -S 0x1588
	ipcrm -S 0x19d4
	ipcrm -S 0x461
	ipcrm -S 0x1591
	ipcrm -S 0x462
	ipcrm -S 0xcfa
	ipcrm -S 0x26c2
	ipcrm -S 0x1592
	ipcrm -S 0x19de
	ipcrm -S 0x2b68
	ipcrm -S 0x26b8
	ipcrm -S 0x2b72
	ipcrm -S 0x56cd
	ipcrm -S 0x56c3
