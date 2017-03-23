#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "AES_common.h"
#include "AES_sbox.h"
#include "multiplication.h"
#include "subspace_checks.h"

#define NUMBER_TEST 1
#define NUMBER_CP 3


word8 randomByte(){
  return (word8) randomInRange(0, 255);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

word8 usefulCalculation(word8 a, word8 b, word8 key)
{
    word8 r;

    r = inverseByteTransformation(a ^ key) ^ inverseByteTransformation(b ^ key);

    return r;
}

/**Attack 3 rounds of AES using the subspace trail and 2 texts*/
int attack3Rounds_2CP(unsigned char initialKey[4][4])
{
    unsigned char p[2][16], c[2][16], test1[4][4], test2[4][4], temp[2];
    word8 indice, indice1, temp2, temp3;

    int i, j, k, l, flag1, flag2, flag3, cont = 0;

    //Generation of the plaintexts in the same coset of D
    for(i=0;i<16;i++)
    {
        p[0][i] = randomByte();
        p[1][i] = p[0][i];
    }
    p[1][0] = randomByte();
    p[1][5] = randomByte();
    p[1][10] = randomByte();
    p[1][15] = randomByte();

    //encryption
    for(j=0;j<2;j++)
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

    //attack on the first column
    for(i=0;i<256;i++)
    {
        indice = (word8) i;

        temp[1] = usefulCalculation(c[0][13], c[1][13], indice);

        flag1 = 0;

        for(j=0; j<256; j++)
        {
            indice1 = (word8) j;
            temp2 = usefulCalculation(c[0][0], c[1][0], indice1);

            if(temp2 == multiplicationX(temp[1]))
            {
                flag1++;
            }
        }

        flag2 = 0;

        for(j=0; ((j<256)&&(flag1 != 0)); j++)
        {
            indice1 = (word8) j;
            temp2 = usefulCalculation(c[0][10], c[1][10], indice1);

            if(temp2 == temp[1])
            {
                flag2++;
            }
        }

        flag3 = 0;

        for(j=0; ((j<256)&&(flag1 != 0)&&(flag2 != 0)); j++)
        {
            indice1 = (word8) j;
            temp2 = usefulCalculation(c[0][7], c[1][7], indice1);

            if(temp2 == (multiplicationX(temp[1]) ^ temp[1]))
            {
                flag3++;
            }
        }

        if((flag3!=0)&&(flag2!=0)&&(flag1!=0))
        {
            cont = cont + flag3 * flag2 * flag1;
        }
    }

    printf("Number of found key for each column with a single pair of chosen plaintexts: %d\n", cont);
    printf("Theoretical value: 256.\n\n");
}

/**Attack 3 rounds of AES using the subspace trail and 3 texts*/
int attack3Rounds_3CP(unsigned char initialKey[4][4])
{
    unsigned char p[NUMBER_CP][16], c[NUMBER_CP][16], test1[4][4], test2[4][4], temp[NUMBER_CP], FinalKey[4][4];
    word8 indice, indice1, temp2, temp3, memory1[256], memory2[256], memory3[256];

    int i, j, k, l, flag1, flag2, flag3, flag11 = 0, i1, i2, i3;

    for(i=0;i<4;i++)
    {
        for(k=0;k<4;k++)
            FinalKey[i][k] = initialKey[i][k];
    }

    for(j = 0; j < 3; j++)
        generationRoundKey(&(FinalKey[0][0]), j);


    srand(time(NULL));

    //Generation of the plaintexts in the same coset of D
    for(i=0;i<16;i++)
    {
        p[0][i] = randomByte();
        for(j=1;j<NUMBER_CP;j++)
        {
            p[j][i] = p[0][i];
        }
    }
    for(j=1;j<NUMBER_CP;j++)
    {
        p[j][0] = randomByte();
        p[j][5] = randomByte();
        p[j][10] = randomByte();
        p[j][15] = randomByte();
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

    printf("KEY OF THE FINAL ROUND.\n");
    printf("Position of the Bytes:\n0-th - 7th - 10th - 13th\n\n");
    printf("Correct key:\n");
    printf("%02x - %02x - %02x - %02x\n\n", FinalKey[0][0], FinalKey[3][1], FinalKey[2][2], FinalKey[1][3]);
    printf("FOUND keys:\n");

    //attack only the first column of SR(k)
    for(i=0;i<256;i++)
    {
        indice = (word8) i;

        for(j=1;j<NUMBER_CP;j++)
            temp[j] = usefulCalculation(c[0][13], c[j][13], indice);

        flag1 = 0;

        for(j=0; j<256; j++)
        {
            indice1 = (word8) j;

            k = 1;

            do{

                flag11 = 0;

                temp2 = usefulCalculation(c[0][0], c[k][0], indice1);

                if(temp2 == multiplicationX(temp[k]))
                {
                    flag11 = 1;
                }

                k++;

            }while((k<NUMBER_CP)&&(flag11 != 0));

            if(flag11 != 0)
            {
                memory1[flag1] = indice1;
                flag1++;
            }
        }

        flag2 = 0;

        for(j=0; ((j<256)&&(flag1 != 0)); j++)
        {
            indice1 = (word8) j;

            k = 1;

            do{

                flag11 = 0;

                temp2 = usefulCalculation(c[0][10], c[k][10], indice1);

                if(temp2 == temp[k])
                {
                    flag11 = 1;
                }

                k++;

            }while((k<NUMBER_CP)&&(flag11 != 0));

            if(flag11 != 0)
            {
                memory2[flag2] = indice1;
                flag2++;
            }

        }

        flag3 = 0;

        for(j=0; ((j<256)&&(flag1 != 0)&&(flag2 != 0)); j++)
        {
            indice1 = (word8) j;

            k = 1;

            do{

                flag11 = 0;

                temp2 = usefulCalculation(c[0][7], c[k][7], indice1);

                if(temp2 == (multiplicationX(temp[k]) ^ temp[k]))
                {
                    flag11 = 1;
                }

                k++;

            }while((k<NUMBER_CP)&&(flag11 != 0));

            if(flag11 != 0)
            {
                memory3[flag3] = indice1;
                flag3++;
            }
        }

        i1 = 0;
        i2 = 0;
        i3 = 0;

        if((i1<flag1)&&(i2<flag2)&&(i3<flag1))
        {
            printf("%02x - %02x - %02x - %02x\n\n", memory1[i1], memory3[i3], memory2[i2], indice);

            i1++;
            if(i1==flag1)
            {
                i1 = 0;
                i2++;
                if(i2==flag2)
                {
                    i2 = 0;
                    i3++;
                }
            }
        }
    }

}

int main()
{
    unsigned char initialKey[4][4] = {
        0x00, 0x04, 0x08, 0x0c,
        0x01, 0x05, 0x09, 0x0d,
        0x02, 0x06, 0x0a, 0x0e,
        0x03, 0x07, 0x0b, 0x0f
    };

    int i, j;

    srand(time(NULL));

    printf("ATTACK ON 3 ROUNDS:\n\n- 2 Chosen Plaintexts: given 2 CP, this attack finds all the combinations for the first column.");
    printf("It returns the number of found combinations against the theoretical one.\n\n");
    printf("- 3 Chosen Plaintexts: given 3 CP, this attack finds the exact combination for the first column.\n\n");
    printf("Digit 2 if you want to test the 2 CP attack or 3 for the 3 CP attack:\n");

    scanf("%d", &i);

    printf("\n");
    /*In this attack, only 2 Chosen Plaintexts in the same coset of Dare used. The output is the number of key combinations found for one column.
    Remember that the theoretical value is 256.*/

    for(j=0;j<NUMBER_TEST; j++)
    {
        printf("Test: %d\n", j+1);
        if(i==2)
            attack3Rounds_2CP(initialKey);
        else
            attack3Rounds_3CP(initialKey);
    }

    return 0;
}






