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

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

word8 usefulCalculation(word8 a, word8 b, word8 key)
{
    word8 r;

    r = inverseByteTransformation(a ^ key) ^ inverseByteTransformation(b ^ key);

    return r;
}

///**Attack 1 round of AES using Subspace Trail */
//int attack1Round(unsigned char initialKey[4][4])
//{
//    unsigned char p[NUMBER_CP][16], c[NUMBER_CP][16], test1[4][4], test2[4][4], temp[NUMBER_CP], FinalKey[4][4];
//    word8 indice, indice1, temp2, temp3, memory1[256], memory2[256], memory3[256];
//
//    int i, j, k, l, flag1, flag2, flag3, flag11 = 0, i1, i2, i3;
//
//    for(i=0;i<4;i++)
//    {
//        for(k=0;k<4;k++)
//            FinalKey[i][k] = initialKey[i][k];
//    }
//
//    generationRoundKey(&(FinalKey[0][0]), 0);
//
//    srand(time(NULL));
//
//    //Generation of the plaintexts in the same coset of M
//    for(i=0;i<16;i++)
//    {
//        p[0][i] = randomByte();
//        for(j=1;j<NUMBER_CP;j++)
//        {
//            p[j][i] = p[0][i];
//        }
//    }
//    for(j=0;j<NUMBER_CP;j++)
//    {
//        p[j][0] = (word8) j;
//        p[j][7] = randomByte();
//        p[j][10] = (word8) j;
//        p[j][13] = randomByte();
//    }
//
//    //encryption
//    for(j=0;j<NUMBER_CP;j++)
//    {
//        for(i=0;i<4;i++)
//        {
//            for(k=0;k<4;k++)
//                test1[i][k] = p[j][i+4*k];
//        }
//
//        mixColumn(&(test1[0][0]));
//
//        encryption1Round(test1, initialKey, &(test2[0][0]));
//
//        for(i=0;i<4;i++)
//        {
//            for(k=0;k<4;k++)
//            {
//                p[j][i+4*k] = test1[i][k];
//                c[j][i+4*k] = test2[i][k];
//            }
//        }
//
//    }
//
//    inverseMixColumn(&(FinalKey[0][0]));
//    for(j=0;j<NUMBER_CP;j++)
//    {
//        for(i=0;i<4;i++)
//        {
//            for(k=0;k<4;k++)
//                test1[i][k] = c[j][i+4*k];
//        }
//
//        inverseMixColumn(&(test1[0][0]));
//
//        for(i=0;i<4;i++)
//        {
//            for(k=0;k<4;k++)
//            {
//                c[j][i+4*k] = test1[i][k];
//            }
//        }
//
//    }
//
//    printf("KEY OF THE FINAL ROUND.\n");
//    printf("Position of the Bytes:\n0-th - 7th - 10th - 13th\n\n");
//    printf("Correct key:\n");
//    printf("%x - %x - %x - %x\n\n", FinalKey[0][0], FinalKey[3][1], FinalKey[2][2], FinalKey[1][3]);
//    printf("FOUND keys:\n");
//
//    //attack only on the first column of SR(k) - use only 3 texts!
//    for(i=0;i<256;i++)
//    {
//        indice = (word8) i;
//
//        for(j=1;j<NUMBER_CP;j++)
//            temp[j] = usefulCalculation(c[0][13], c[j][13], indice);
//
//        flag1 = 0;
//
//        for(j=0; j<256; j++)
//        {
//            indice1 = (word8) j;
//
//            k = 1;
//
//            do{
//
//                flag11 = 0;
//
//                temp2 = usefulCalculation(c[0][0], c[k][0], indice1);
//
//                if(temp2 == multiplicationX(temp[k]))
//                {
//                    flag11 = 1;
//                }
//
//                k++;
//
//            }while((k<NUMBER_CP)&&(flag11 != 0));
//
//            if(flag11 != 0)
//            {
//                memory1[flag1] = indice1;
//                flag1++;
//            }
//        }
//
//        flag2 = 0;
//
//        for(j=0; j<256; j++)
//        {
//            indice1 = (word8) j;
//
//            k = 1;
//
//            do{
//
//                flag11 = 0;
//
//                temp2 = usefulCalculation(c[0][10], c[k][10], indice1);
//
//                if(temp2 == temp[k])
//                {
//                    flag11 = 1;
//                }
//
//                k++;
//
//            }while((k<NUMBER_CP)&&(flag11 != 0));
//
//            if(flag11 != 0)
//            {
//                memory2[flag2] = indice1;
//                flag2++;
//            }
//
//        }
//
//        flag3 = 0;
//
//        for(j=0; j<256; j++)
//        {
//            indice1 = (word8) j;
//
//            k = 1;
//
//            do{
//
//                flag11 = 0;
//
//                temp2 = usefulCalculation(c[0][7], c[k][7], indice1);
//
//                if(temp2 == (multiplicationX(temp[k]) ^ temp[k]))
//                {
//                    flag11 = 1;
//                }
//
//                k++;
//
//            }while((k<NUMBER_CP)&&(flag11 != 0));
//
//            if(flag11 != 0)
//            {
//                memory3[flag3] = indice1;
//                flag3++;
//            }
//        }
//
//        i1 = 0;
//        i2 = 0;
//        i3 = 0;
//
//        if((i1<flag1)&&(i2<flag2)&&(i3<flag1))
//        {
//            printf("%x - %x - %x - %x\n\n", memory1[i1], memory3[i3], memory2[i2], indice);
//
//            i1++;
//            if(i1==flag1)
//            {
//                i1 = 0;
//                i2++;
//                if(i2==flag2)
//                {
//                    i2 = 0;
//                    i3++;
//                }
//            }
//        }
//    }
//
//}

/**Attack on 2 rounds of AES using subspace trail */
int attack2Rounds(unsigned char initialKey[4][4])
{
    unsigned char p[NUMBER_CP][16], c[NUMBER_CP][16], test1[4][4], test2[4][4], temp[NUMBER_CP], FinalKey[4][4];
    word8 indice, indice1, temp2, temp3, memory1[256], memory2[256], memory3[256];

    int i, j, k, l, flag1, flag2, flag3, flag11 = 0, i1, i2, i3;

    for(i=0;i<4;i++)
    {
        for(k=0;k<4;k++)
            FinalKey[i][k] = initialKey[i][k];
    }

    for(j = 0; j < 2; j++)
        generationRoundKey(&(FinalKey[0][0]), j);

    //Generation of the plaintexts in the same coset of C
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
        p[j][0] = (word8) j;
        p[j][1] = randomByte();
        p[j][2] = (word8) j;
        p[j][3] = randomByte();
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
    printf("%x - %x - %x - %x\n\n", FinalKey[0][0], FinalKey[3][1], FinalKey[2][2], FinalKey[1][3]);
    printf("FOUND keys:\n");

    //attack only the first column of the SR(key) - suppose to work only with 3 texts!
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

        for(j=0; j<256; j++)
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

        for(j=0; j<256; j++)
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
            printf("%x - %x - %x - %x\n\n", memory1[i1], memory3[i3], memory2[i2], indice);

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

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

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

    printf("ATTACK ON 2 ROUNDS.\n\n");

     printf("\n");

    for(j = 0; j< NUMBER_TEST; j++)
    {
        printf("TEST: %d\n", j+1);
        attack2Rounds(initialKey);
    }

    return 0;
}






