CFLAGS=-O3 -Wall

all: AES 2RoundsAttack_AES 3RoundsAttack_AES_3CP 4RoundsAttack_AES_End

multiplication: multiplication.h multiplication.c
	$(CC) $(CFLAGS) -c -o multiplication.o multiplication.c
	
subspace_checks: subspace_checks.h subspace_checks.c
	$(CC) $(CFLAGS) -c -o subspace_checks.o subspace_checks.c

aes_common: AES_common.h AES_common.c
	$(CC) $(CFLAGS) -c -o AES_common10.o AES_common.c -DN_Round=10
	$(CC) $(CFLAGS) -c -o AES_common2.o AES_common.c -DN_Round=2
	$(CC) $(CFLAGS) -c -o AES_common3.o AES_common.c -DN_Round=3
	$(CC) $(CFLAGS) -c -o AES_common4.o AES_common.c -DN_Round=4


AES: aes_common multiplication AES.c
	$(CC) $(CFLAGS) -o AES AES.c AES_common10.o multiplication.o

2RoundsAttack_AES: aes_common multiplication subspace_checks 2RoundsAttack_AES.c
	$(CC) $(CFLAGS) -o 2RoundsAttack_AES 2RoundsAttack_AES.c AES_common2.o subspace_checks.o multiplication.o

3RoundsAttack_AES_3CP: aes_common multiplication subspace_checks 3RoundsAttack_AES_3CP.c
	$(CC) $(CFLAGS) -o 3RoundsAttack_AES_3CP 3RoundsAttack_AES_3CP.c AES_common3.o subspace_checks.o multiplication.o

4RoundsAttack_AES_End: aes_common multiplication subspace_checks 4RoundsAttack_AES_End.c
	$(CC) $(CFLAGS) -o 4RoundsAttack_AES_End 4RoundsAttack_AES_End.c AES_common4.o subspace_checks.o multiplication.o


clean:
	$(RM) -f AES_common10.o AES_common2.o AES_common3.o AES_common4.o 
	$(RM) -f multiplication.o 
	$(RM) -f subspace_checks.o
	$(RM) -f AES
	$(RM) -f 3RoundsAttack_AES_3CP
	$(RM) -f 2RoundsAttack_AES
	$(RM) -f 4RoundsAttack_AES_End
	
.PHONY: clean
