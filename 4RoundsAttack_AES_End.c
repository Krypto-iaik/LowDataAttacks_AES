#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "AES_common.h"
#include "AES_sbox.h"
#include "multiplication.h"
#include "subspace_checks.h"

#define NUMBER_CP 3
#define PRINT 1 /** 1 to print - 0 no print */

word8 randomByte(){
  return (word8) randomInRange(0, 255);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

word8 usefulCalculation(word8 a, word8 b, word8 key)
{
    word8 r;

    r = inverseByteTransformation(a ^ key) ^ inverseByteTransformation(b ^ key);

    return r;
}

word8 usefulCalculation2(word8 a, int i)
{
    word8 b;
    b = multiplicationX(a);

    if(i == 1)
        return b;

    if(i == 2)
        return (b^a);

    return a;
}

int supertest(unsigned char cipher2[][4], unsigned char cipher1[][4], unsigned char *salvare1, unsigned char *salvare2, int ciao, int ciao2, int z, int zz)
{
    int cont = 0, k, j, flag, l;
    word8 indice, indice1, valore, test[NUMBER_CP];

    for(k=0;k<256;k++)
    {
        indice = (word8) k;

        for(j=0;j<NUMBER_CP;j++)
            test[j] = usefulCalculation(cipher2[0][z], cipher2[j][z], indice);

        for(j=0;j<256;j++)
        {
            indice1 = (word8) j;

            l=1;

            do{
                flag = 0;
                valore = usefulCalculation(cipher1[0][zz], cipher1[l][zz], indice1);
                if(usefulCalculation2(valore, ciao) == usefulCalculation2(test[l], ciao2))
                {
                    flag=1;
                }
                l++;
            }while((l<NUMBER_CP)&&(flag==1));

            if(flag == 1)
            {
                *(salvare1 + cont) = indice;
                *(salvare2 + cont) = indice1;
                cont++;
            }
        }
    }

    cont = cont - 1;

    return cont;
}

void mixColumnMod(word8 *p){

  int i, j;
  word8 colonna[4], nuovaColonna[4];

  //separo le colonne e calcolo le nuove
  for(i=0;i<4;i++){
    colonna[i]=*(p + i);
  }

    //calcolo nuova colonna
    nuovaColonna[0]= multiplicationX(colonna[0]) ^ multiplicationX(colonna[1]) ^ colonna[1] ^ colonna[2] ^ colonna[3];
    nuovaColonna[1]= colonna[0] ^ multiplicationX(colonna[1]) ^ multiplicationX(colonna[2]) ^ colonna[2] ^ colonna[3];
    nuovaColonna[2]= colonna[0] ^ colonna[1] ^ multiplicationX(colonna[2]) ^ multiplicationX(colonna[3]) ^ colonna[3];
    nuovaColonna[3]= multiplicationX(colonna[0]) ^ colonna[0] ^ colonna[1] ^ colonna[2] ^ multiplicationX(colonna[3]);

    //reinserisco colonna
  for(j=0;j<4;j++){
      *(p + j)=nuovaColonna[j];
  }

}



//inverse on a single column
void inverseMixColumnMod(word8 *p){

  int i, j;
  word8 colonna[4], nuovaColonna[4];

  //separo le colonne e calcolo le nuove
  for(i=0;i<4;i++)
    colonna[i]=*(p + i);

    //calcolo nuova colonna
    nuovaColonna[0]= multiplicationXN(colonna[0], 3) ^ multiplicationXN(colonna[0], 2) ^ multiplicationX(colonna[0]) ^
                     multiplicationXN(colonna[1], 3) ^ multiplicationX(colonna[1]) ^ colonna[1] ^ multiplicationXN(colonna[2], 3) ^
                     multiplicationXN(colonna[2], 2) ^ colonna[2] ^ multiplicationXN(colonna[3], 3) ^ colonna[3];

    nuovaColonna[1]= multiplicationXN(colonna[0], 3) ^ colonna[0] ^ multiplicationXN(colonna[1], 3) ^ multiplicationXN(colonna[1], 2) ^
                     multiplicationX(colonna[1]) ^ multiplicationXN(colonna[2], 3) ^ multiplicationX(colonna[2]) ^ colonna[2] ^
                     multiplicationXN(colonna[3], 3) ^ multiplicationXN(colonna[3], 2) ^ colonna[3];

    nuovaColonna[2]= multiplicationXN(colonna[0], 3) ^ multiplicationXN(colonna[0], 2) ^ colonna[0] ^ multiplicationXN(colonna[1], 3) ^
                    colonna[1] ^ multiplicationXN(colonna[2], 3) ^ multiplicationXN(colonna[2], 2) ^ multiplicationX(colonna[2]) ^
                    multiplicationXN(colonna[3], 3)^multiplicationX(colonna[3]) ^ colonna[3];

    nuovaColonna[3]= multiplicationXN(colonna[0], 3)^ multiplicationX(colonna[0]) ^ colonna[0] ^ multiplicationXN(colonna[1], 3) ^
                    multiplicationXN(colonna[1], 2)^colonna[1] ^ multiplicationXN(colonna[2], 3)^colonna[2] ^ multiplicationXN(colonna[3], 3)^
                    multiplicationXN(colonna[3], 2)^multiplicationX(colonna[3]);

    //reinserisco colonna
    for(j=0;j<4;j++){
      *(p + j)=nuovaColonna[j];
    }

}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/**Attack 4 rounds of AES - using the extension at the end*/
int attack4Rounds_3CP(unsigned char initialKey[4][4])
{
    unsigned char p[NUMBER_CP][16], c[NUMBER_CP][16], FinalKey[4][4], test1[4][4], test2[4][4], cipher1[NUMBER_CP][4], cipher2[NUMBER_CP][4];
    word8 prova, prova2, test[4], PreFinalKey[4][4], verifica, rCostante;

    word8 indice, indice1, valore, flag, PreFinalKey2[4][4], after[4], after2[4];
    word8 salvare11[512], salvare1[512], salvare22[512], salvare2[512], salvare00[512], salvare0[512], salvare33[512], salvare3[512];

    int j, k, cont, l, val1, val2, val11, val22, cont1, cont2, cont3, superflag = 0, cont00, cont11, cont22, cont33, ciccio;
    long int supercontatore = 0, supercontatore2= 0, i;

    for(i=0;i<4;i++)
    {
        for(k=0;k<4;k++)
            PreFinalKey[i][k] = initialKey[i][k];
    }

    for(i=0;i<4;i++)
    {
        for(k=0;k<4;k++)
            PreFinalKey2[i][k] = PreFinalKey[i][k];
    }

    for(j = 0; j < 3; j++)
        generationRoundKey(&(PreFinalKey[0][0]), j);

    for(i=0;i<4;i++)
    {
        for(k=0;k<4;k++)
            FinalKey[i][k] = initialKey[i][k];
    }
    for(j = 0; j < 4; j++)
        generationRoundKey(&(FinalKey[0][0]), j);

    inverseMixColumn(&(PreFinalKey[0][0]));
    inverseShiftRows(&(PreFinalKey[0][0]));

    rCostante=0x02;
    for(i=1;i<3;i++)
        rCostante=multiplicationX(rCostante);

    //Generation of the plaintexts in the same coset of D
    for(i=0;i<16;i++)
    {
        p[0][i] = randomByte();
        for(j=1;j<NUMBER_CP;j++)
        {
            p[j][i] = p[0][i];
        }
    }
    for(j=0;j<NUMBER_CP;j++)
    {
        p[j][0] = (word8) j;
        p[j][5] = randomByte();
        p[j][10] = randomByte();
        p[j][15] = (word8) j;
    }

    //encryption
    for(j=0;j<NUMBER_CP;j++)
    {
        for(i=0;i<4;i++)
        {
            for(k=0;k<4;k++)
                test1[i][k] = p[j][i+4*k];
        }

        encryption(test1, initialKey, &(test2[0][0]));

        for(i=0;i<4;i++)
        {
            for(k=0;k<4;k++)
                c[j][i+4*k] = test2[i][k];
        }

    }

    if(PRINT == 1)
        printf("Found key that satisfy the key schedule:\n");

    for(i=0;i<65536;i++)
    {
        prova = (word8) ((int) (i%256));
        prova2 = (word8) ((int) ((i-(i%256))/256));

        for(k=0;k<NUMBER_CP;k++)
        {
            cipher1[k][0] = inverseByteTransformation(c[k][0] ^ prova);
            cipher1[k][1] = inverseByteTransformation(c[k][13]^ FinalKey[1][3]);
            cipher1[k][2] = inverseByteTransformation(c[k][10] ^ FinalKey[2][2]);
            cipher1[k][3] = inverseByteTransformation(c[k][7] ^ FinalKey[3][1]);
        }

        for(k=0;k<NUMBER_CP;k++)
        {
            cipher2[k][0] = inverseByteTransformation(c[k][12] ^ FinalKey[0][3]);
            cipher2[k][1] = inverseByteTransformation(c[k][9] ^ FinalKey[1][2]);
            cipher2[k][2] = inverseByteTransformation(c[k][6] ^ FinalKey[2][1]);
            cipher2[k][3] = inverseByteTransformation(c[k][3] ^ prova2);
        }

        //MixColumn-1
        for(k=0;k<NUMBER_CP;k++)
        {
            for(l=0;l<4;l++)
                test[l] = cipher2[k][l];

            inverseMixColumnMod(&(test[0]));

            for(l=0;l<4;l++)
                cipher2[k][l] = test[l];
        }
        for(k=0;k<NUMBER_CP;k++)
        {
            for(l=0;l<4;l++)
                test[l] = cipher1[k][l];

            inverseMixColumnMod(&(test[0]));

            for(l=0;l<4;l++)
                cipher1[k][l] = test[l];
        }

        val1 = 0;
        val2 = 1;
        val11 = 1; //cipher 2
        val22 = 0;
        cont = -1;

        cont = supertest(cipher2, cipher1, &(salvare0[0]), &(salvare00[0]), val1, val2, val11, val22);
        cont00 = cont;

        if(cont>=0)
        {
            val1 = 2;
            val2 = 0;
            val11 = 2; //cipher 2
            val22 = 1;
            cont1 = -1;

            cont1 = supertest(cipher2, cipher1, &(salvare1[0]), &(salvare11[0]), val1, val2, val11, val22);
            cont11 = cont1;

            if(cont1>=0)
            {

                val1 = 0;
                val2 = 1;
                val11 = 3; //cipher 2
                val22 = 2;
                cont2 = -1;

                cont2 = supertest(cipher2, cipher1, &(salvare2[0]), &(salvare22[0]), val1, val2, val11, val22);
                cont22 = cont2;

                if(cont2>=0)
                {

                    val1 = 2;
                    val2 = 0;
                    val11 = 0; //cipher 2
                    val22 = 3;
                    cont3 = -1;

                    cont3 = supertest(cipher2, cipher1, &(salvare3[0]), &(salvare33[0]), val1, val2, val11, val22);
                    cont33 = cont3;
                }
            }

        }

         while((cont>=0)&&(cont2>=0)&&(cont1>=0)&&(cont3>=0))
        {
            //Note that the inverse-mixColumn operation is applied to the key of the 3 round!
            after[0] = salvare00[cont];
            after[1] = salvare11[cont1];
            after[2] = salvare22[cont2];
            after[3] = salvare33[cont3];

            mixColumnMod(&(after[0]));

            after2[0] = salvare3[cont3];
            after2[1] = salvare0[cont];
            after2[2] = salvare1[cont1];
            after2[3] = salvare2[cont2];

            mixColumnMod(&(after2[0]));

            verifica = prova ^ byteTransformation(after2[1]) ^ after[0] ^ rCostante;

            if(verifica == 0)
            {
                verifica = prova2 ^ byteTransformation(after2[0]) ^ after[3];

                if(verifica == 0)
                {
                    supercontatore2++;

                    if(PRINT==1)
                    {
                        printf("FINAL KEY - k_4:\n");
                        printf("nibble in position (0,0) - Real value: 0x%02x - Found value: 0x%02x\n", FinalKey[0][0], prova);
                        printf("nibble in position (3,0) - Real value: 0x%02x - Found value: 0x%02x\n\n", FinalKey[3][0], prova2);

                        printf("KEY MC^{-1}(k_3) - the inverse MixColumn is applied to k_3:\n");
                        printf("First column:\n");
                        printf("Real Values: %x - %x - %x - %x\n", PreFinalKey[0][0], PreFinalKey[1][1], PreFinalKey[2][2], PreFinalKey[3][3]);
                        printf("Found Values: %x - %x - %x - %x\n", salvare00[cont], salvare11[cont1], salvare22[cont2], salvare33[cont3]);
                        printf("Fourth column:\n");
                        printf("Real Values: %x - %x - %x - %x\n", PreFinalKey[0][3], PreFinalKey[1][0], PreFinalKey[2][1], PreFinalKey[3][2]);
                        printf("Found Values: %x - %x - %x - %x\n\n", salvare3[cont3], salvare0[cont], salvare1[cont1], salvare2[cont2]);

                        if((prova == FinalKey[0][0])&&(prova2 == FinalKey[3][0])&&
                            (PreFinalKey[0][0]==salvare00[cont])&&(PreFinalKey[1][0]==salvare0[cont])&&
                            (PreFinalKey[1][1]==salvare11[cont1])&&(PreFinalKey[2][1]==salvare1[cont1])&&
                            (PreFinalKey[2][2]==salvare22[cont2])&&(PreFinalKey[3][2]==salvare2[cont2])&&
                            (PreFinalKey[3][3]==salvare33[cont3])&&(PreFinalKey[0][3]==salvare3[cont3]))
                        {
                            printf("The previous key is the RIGHT ONE\n\n");
                        }
                        else
                        {
                            printf("The previous key is WRONG!\n\n");
                        }
                    }
                }
            }

            supercontatore++;

            if((prova == FinalKey[0][0])&&(prova2 == FinalKey[3][0])&&
               (PreFinalKey[0][0]==salvare00[cont])&&(PreFinalKey[1][0]==salvare0[cont])&&
               (PreFinalKey[1][1]==salvare11[cont1])&&(PreFinalKey[2][1]==salvare1[cont1])&&
               (PreFinalKey[2][2]==salvare22[cont2])&&(PreFinalKey[3][2]==salvare2[cont2])&&
               (PreFinalKey[3][3]==salvare33[cont3])&&(PreFinalKey[0][3]==salvare3[cont3]))
            {
                superflag=1;
            }

            cont = cont - 1;
            if(cont < 0)
            {
                cont = cont00;
                cont1 = cont1 - 1;
                if(cont1 < 0)
                {
                    cont1 = cont11;
                    cont2 = cont2 - 1;
                    if(cont2 < 0)
                    {
                        cont2 = cont22;
                        cont3 = cont3 - 1;
                    }
                }
            }
        }
    }

    printf("Number of found key BEFORE the key schedule: %d. Theoretical values: 65356.\n\n", supercontatore);
    printf("Number of found key AFTER the key schedule: %d. Theoretical values: 1.\n\n", supercontatore2);

    if(superflag==1)
        printf("Right key FOUND among all the possibility -> Brute Force Attack!\n PERFECT Test!\n\n");
    else
        printf("Test FAIL!\n");

}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int main()
{
    unsigned char initialKey[4][4] = {
        0x00, 0x04, 0x08, 0x0c,
        0x01, 0x05, 0xe9, 0xcd,
        0x02, 0x18, 0x0a, 0x0e,
        0x03, 0xa7, 0x0b, 0x1f
    };

    srand(time(NULL));

    printf("ATTACK 4 ROUNDS of AES - EXTENSION AT THE END.\n\n");
    printf("In this attack, we guess 8 bytes of the final key. Then we found 8 byte of the key of the third round.");
    printf("Finally, we guess the remaining the last 3 bytes of the final key and we implement a brute force attack.\n");
    printf("For simplicity, we guess only 2 bytes of the final key, and we check that among the found combinations, the right one is present.\n\n");

    attack4Rounds_3CP(initialKey);

    return 0;
}






