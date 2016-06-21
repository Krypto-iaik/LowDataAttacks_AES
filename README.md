Low-Data Attacks for AES-128 up to 4 rounds. The attacks are set up using the Subspace Trail.

For a complete discussion about these attacks using the Subspace Trail, we refer to the following paper:

Title: "Subspace Trail Cryptanalysis and its Applications to AES" Authors: Lorenzo Grassi, Christian Rechberger and Sondre Ronjom Available at: http://eprint.iacr.org/

Programs:

0) AES.c

Attacks:

1) 1-2RoundsAttack_AES.c
2) 3RoundsAttack_AES_3CP.c
3) 4RoundsAttack_AES_End.c

The programs should run with almost C-compilers (we have used gcc version 4.8.1). None of these programs is speed-optimized, they are for verification purposes only.

In all the programs, the main function is always the last one.

The first program contains our implementation of AES (encryption and decryption). It has been verified against test-vectors.

The other four programs are the ATTACKs up to four rounds.

For each program, the user can choose the secret key (which is defined in the main fuction).
Only for simplicity, the programs don't try to find the complete secret key, but only part of it.
We think that this is sufficient, since in all the cases the remaing part of the key can be found using the same implementation (and in the case of (3) using a brute force attack).

As we said in the paper, there are two ways to find the keys:
- using S-Box look-ups and no precompuation;
- using precomputations and table look-ups.
The two method are completely equivalent.
For simplicity, we have tested only the first one. 

Detailed description of each of the previous programs (for all the following programs, it is possible to change the secret key defined in the main part,
which is always the last one):

1) It verifies the attack on 1-2 rounds (Sect. 6.2) using 3 Chosen Plaintexts (the user can choose which attack to verify) - dim(M) = dim(C) = 4.
The program generates 3 pairs of chosen plaintexts in the same coset of M (for 1 round) and C (for 2 rounds).
For simplicity, it finds one column of the final key, and it checks that it is the right one.
It also contains an implementation of 1 and round AES.

It is possible to repeat the test more times, using the variable NUMBER_TEST at line 6.

2) It verifies the attack on 3 rounds AES (Sect. 6.1) - dim(D) = 4.
The user can choose to use 2 or 3 Chosen Plaintexts for the attack.
In the case of 3 CP, as before, the programs generates 3 pairs of CP, finds one column of the final key and checks that it is right.
In the case of 2 CP, it generates 2 pairs of CP, finds all the possible values of one column of the final key (on average 256), and checks that the
right key is present.

Also in this case, it is possible to repeat the test more times, using the variable NUMBER_TEST at line 6.

3) It verifies the attack on 4 rounds AES (Sect. 8), based on the attack on 3 rounds with the extension at the End (dim(D) = 4). 
Theoretically, the attacker has to guess eight bytes of the final key.
For simplicity, she guesses only two of them, while the other six are imposed equal to the right values.
Then, the program generates 3 CP in the same cosets of D and the corresponding ciphertexts.
The program partially decrypts the ciphertexts (using the guesses values), and repeats the previous attack on 3 rounds.
That is, it finds one column of the secret key.
Finally, it checks that the right column is found when the two guessed bytes of the final key are the right ones.

Theoretically, the program has to find all the columns, guess the remaining 3 bytes of the final key and do a brute force attack.
However, we believe that the program is sufficient to test our attack.
The time of execution is about 90-120 sec on a Desktop PC.

Finally, it is possible to print the keys that satisfy the key schedule (using variable PRINT at line 7).
Since we guess only 2 bytes of the final key, on average the program finds 2^16 possible keys. Since the key schedule is satisfied with probability 2^-16, then only the right key satisfied it.
