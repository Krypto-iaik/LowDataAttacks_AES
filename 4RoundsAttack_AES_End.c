#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define N_Round 4
#define NUMBER_CP 3
#define PRINT 1 /** 1 to print - 0 no print */

typedef unsigned char word8;

//S-box
const unsigned char sBox[256] = {
  0x63, 0x7C, 0x77, 0x7B, 0xF2, 0x6B, 0x6F, 0xC5, 0x30, 0x01, 0x67, 0x2B, 0xFE, 0xD7, 0xAB, 0x76,
  0xCA, 0x82, 0xC9, 0x7D, 0xFA, 0x59, 0x47, 0xF0, 0xAD, 0xD4, 0xA2, 0xAF, 0x9C, 0xA4, 0x72, 0xC0,
  0xB7, 0xFD, 0x93, 0x26, 0x36, 0x3F, 0xF7, 0xCC, 0x34, 0xA5, 0xE5, 0xF1, 0x71, 0xD8, 0x31, 0x15,
  0x04, 0xC7, 0x23, 0xC3, 0x18, 0x96, 0x05, 0x9A, 0x07, 0x12, 0x80, 0xE2, 0xEB, 0x27, 0xB2, 0x75,
  0x09, 0x83, 0x2C, 0x1A, 0x1B, 0x6E, 0x5A, 0xA0, 0x52, 0x3B, 0xD6, 0xB3, 0x29, 0xE3, 0x2F, 0x84,
  0x53, 0xD1, 0x00, 0xED, 0x20, 0xFC, 0xB1, 0x5B, 0x6A, 0xCB, 0xBE, 0x39, 0x4A, 0x4C, 0x58, 0xCF,
  0xD0, 0xEF, 0xAA, 0xFB, 0x43, 0x4D, 0x33, 0x85, 0x45, 0xF9, 0x02, 0x7F, 0x50, 0x3C, 0x9F, 0xA8,
  0x51, 0xA3, 0x40, 0x8F, 0x92, 0x9D, 0x38, 0xF5, 0xBC, 0xB6, 0xDA, 0x21, 0x10, 0xFF, 0xF3, 0xD2,
  0xCD, 0x0C, 0x13, 0xEC, 0x5F, 0x97, 0x44, 0x17, 0xC4, 0xA7, 0x7E, 0x3D, 0x64, 0x5D, 0x19, 0x73,
  0x60, 0x81, 0x4F, 0xDC, 0x22, 0x2A, 0x90, 0x88, 0x46, 0xEE, 0xB8, 0x14, 0xDE, 0x5E, 0x0B, 0xDB,
  0xE0, 0x32, 0x3A, 0x0A, 0x49, 0x06, 0x24, 0x5C, 0xC2, 0xD3, 0xAC, 0x62, 0x91, 0x95, 0xE4, 0x79,
  0xE7, 0xC8, 0x37, 0x6D, 0x8D, 0xD5, 0x4E, 0xA9, 0x6C, 0x56, 0xF4, 0xEA, 0x65, 0x7A, 0xAE, 0x08,
  0xBA, 0x78, 0x25, 0x2E, 0x1C, 0xA6, 0xB4, 0xC6, 0xE8, 0xDD, 0x74, 0x1F, 0x4B, 0xBD, 0x8B, 0x8A,
  0x70, 0x3E, 0xB5, 0x66, 0x48, 0x03, 0xF6, 0x0E, 0x61, 0x35, 0x57, 0xB9, 0x86, 0xC1, 0x1D, 0x9E,
  0xE1, 0xF8, 0x98, 0x11, 0x69, 0xD9, 0x8E, 0x94, 0x9B, 0x1E, 0x87, 0xE9, 0xCE, 0x55, 0x28, 0xDF,
  0x8C, 0xA1, 0x89, 0x0D, 0xBF, 0xE6, 0x42, 0x68, 0x41, 0x99, 0x2D, 0x0F, 0xB0, 0x54, 0xBB, 0x16
};

//Inverse S-box
const unsigned char inv_s[256] = {
  0x52, 0x09, 0x6A, 0xD5, 0x30, 0x36, 0xA5, 0x38, 0xBF, 0x40, 0xA3, 0x9E, 0x81, 0xF3, 0xD7, 0xFB,
  0x7C, 0xE3, 0x39, 0x82, 0x9B, 0x2F, 0xFF, 0x87, 0x34, 0x8E, 0x43, 0x44, 0xC4, 0xDE, 0xE9, 0xCB,
  0x54, 0x7B, 0x94, 0x32, 0xA6, 0xC2, 0x23, 0x3D, 0xEE, 0x4C, 0x95, 0x0B, 0x42, 0xFA, 0xC3, 0x4E,
  0x08, 0x2E, 0xA1, 0x66, 0x28, 0xD9, 0x24, 0xB2, 0x76, 0x5B, 0xA2, 0x49, 0x6D, 0x8B, 0xD1, 0x25,
  0x72, 0xF8, 0xF6, 0x64, 0x86, 0x68, 0x98, 0x16, 0xD4, 0xA4, 0x5C, 0xCC, 0x5D, 0x65, 0xB6, 0x92,
  0x6C, 0x70, 0x48, 0x50, 0xFD, 0xED, 0xB9, 0xDA, 0x5E, 0x15, 0x46, 0x57, 0xA7, 0x8D, 0x9D, 0x84,
  0x90, 0xD8, 0xAB, 0x00, 0x8C, 0xBC, 0xD3, 0x0A, 0xF7, 0xE4, 0x58, 0x05, 0xB8, 0xB3, 0x45, 0x06,
  0xD0, 0x2C, 0x1E, 0x8F, 0xCA, 0x3F, 0x0F, 0x02, 0xC1, 0xAF, 0xBD, 0x03, 0x01, 0x13, 0x8A, 0x6B,
  0x3A, 0x91, 0x11, 0x41, 0x4F, 0x67, 0xDC, 0xEA, 0x97, 0xF2, 0xCF, 0xCE, 0xF0, 0xB4, 0xE6, 0x73,
  0x96, 0xAC, 0x74, 0x22, 0xE7, 0xAD, 0x35, 0x85, 0xE2, 0xF9, 0x37, 0xE8, 0x1C, 0x75, 0xDF, 0x6E,
  0x47, 0xF1, 0x1A, 0x71, 0x1D, 0x29, 0xC5, 0x89, 0x6F, 0xB7, 0x62, 0x0E, 0xAA, 0x18, 0xBE, 0x1B,
  0xFC, 0x56, 0x3E, 0x4B, 0xC6, 0xD2, 0x79, 0x20, 0x9A, 0xDB, 0xC0, 0xFE, 0x78, 0xCD, 0x5A, 0xF4,
  0x1F, 0xDD, 0xA8, 0x33, 0x88, 0x07, 0xC7, 0x31, 0xB1, 0x12, 0x10, 0x59, 0x27, 0x80, 0xEC, 0x5F,
  0x60, 0x51, 0x7F, 0xA9, 0x19, 0xB5, 0x4A, 0x0D, 0x2D, 0xE5, 0x7A, 0x9F, 0x93, 0xC9, 0x9C, 0xEF,
  0xA0, 0xE0, 0x3B, 0x4D, 0xAE, 0x2A, 0xF5, 0xB0, 0xC8, 0xEB, 0xBB, 0x3C, 0x83, 0x53, 0x99, 0x61,
  0x17, 0x2B, 0x04, 0x7E, 0xBA, 0x77, 0xD6, 0x26, 0xE1, 0x69, 0x14, 0x63, 0x55, 0x21, 0x0C, 0x7D
};

int randomInRange(int min, int max){

  int range = max - min + 1;
  return (min + rand() % range);

}

int randomInRange2(int min, int max){

  int range = max - min + 1;
  int a, b, c;

  a = rand() % range;
  b = rand() % range;
  c = (a*b) % range;
  c = (c + rand()) % range;

  return (min + c);

}

word8 randomByte(){
  return (word8) randomInRange(0, 255);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*Initialization State*/

void initialization(word8 *p, unsigned char initialMessage[][4]){

  int i, j;

  for(i=0; i<4; i++){
    for(j=0; j<4; j++){
      *(p+j+4*i) = initialMessage[i][j];
    }
  }

}

void initialization2(word8 *p, unsigned char initialMessage[][4]){

  int i, j;

  for(i=0; i<4; i++){
    for(j=0; j<4; j++){
      *(p+(N_Round+1)*j+(N_Round+1)*4*i) = initialMessage[i][j];
    }
  }

}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*Add Round Key*/

void addRoundKey(word8 *p, word8 key[][4]){

  int i, j;

  for(i=0; i<4; i++){
    for(j=0; j<4; j++){
      *(p+j+4*i) ^= key[i][j];
    }
  }

}

void addRoundKey2(word8 *p, word8 key[][4][N_Round+1], int costante){

  int i, j;

  for(i=0; i<4; i++){
    for(j=0; j<4; j++){
      *(p+j+4*i) ^= key[i][j][costante];
    }
  }

}

/*Inverse Add round key*/

void invAddRoundKey(word8 *p, word8 key[][4][N_Round+1], int costante){

  word8 keytemp[4][4];

  int i, j;

  for(i=0;i<4;i++){
    for(j=0;j<4;j++)
      keytemp[i][j]=key[i][j][costante];
  }

  inverseMixColumn(&(keytemp[0][0]));

   for(i=0; i<4; i++){
    for(j=0; j<4; j++){
      *(p+j+4*i) ^= keytemp[i][j];
    }
  }

}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*Print*/

void printtt(word8 file[][4]){

  int i, j;

  for(i=0; i<4; i++){
    for(j=0; j<4; j++){
      printf("0x%x, ", file[i][j]);
    }
    printf("\n");
  }

}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*Multiplication times x*/

word8 multiplicationX(word8 byte){

  word8 bitTemp;

  bitTemp=byte>>7;
  byte=byte<<1;

  if(bitTemp==0)
    return byte;
  else
    return (byte^0x1B);

}

/*Multiplication times x^n*/

word8 multiplicationXN(word8 byte, int n){

  int i;

  for(i=0;i<n;i++)
    byte=multiplicationX(byte);

  return byte;

}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*Byte sub transformation with S-box*/

word8 byteTransformation(word8 byte){

  return sBox[byte];

}

/*Inverse byte sub transformation with Inverse S-box*/

word8 inverseByteTransformation(word8 byte){

  return inv_s[byte];

}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*ByteTransformation*/

void byteSubTransformation(word8 *puntatore){

  int i, j;

  for(i=0; i<4; i++){
    for(j=0; j<4; j++)
      *(puntatore+j+4*i)=byteTransformation(*(puntatore+j+4*i));
  }
}

/*Inverse Byte Transformation*/

void inverseByteSubTransformation(word8 *puntatore){

  int i, j;

  for(i=0; i<4; i++){
    for(j=0; j<4; j++)
      *(puntatore+j+4*i)=inverseByteTransformation(*(puntatore+j+4*i));
  }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*Generation Round Key*/

/*Calcolo trasformata terza colonna*/
void nuovaColonna(word8 *pColonna, int numeroRound){

  word8 temp, rCostante, colonnaTemp[4];
  int i;

  //rotazione degli elementi
  temp=*pColonna;
  for(i=0;i<3;i++){
    colonnaTemp[i]=*(pColonna+i+1);
  }
  colonnaTemp[3]=temp;

  //S-box
  for(i=0;i<4;i++)
    colonnaTemp[i]=byteTransformation(colonnaTemp[i]);

  //ultimoStep
  if(numeroRound==0)
    rCostante=0x01;
  else{
    rCostante=0x02;
    for(i=1;i<numeroRound;i++)
       rCostante=multiplicationX(rCostante);
  }

  colonnaTemp[0]^=rCostante;

  //return colonna
  for(i=0;i<4;i++){
    *(pColonna+i)=colonnaTemp[i];
  }

}

void generationRoundKey(word8 *pKey, int numeroRound){

  int i, j;

  word8 colonnaTemp[4];

  //calcolo la trasformata della terza colonna
  for(i=0;i<4;i++)
    colonnaTemp[i]=*(pKey + 3 + 4*i);

  nuovaColonna(&(colonnaTemp[0]), numeroRound);

  //nuova chiave//

  //prima colonna
  for(i=0;i<4;i++)
    *(pKey+4*i)=*(pKey+4*i)^colonnaTemp[i];

  //altre colonne
  for(i=1;i<4;i++){

    for(j=0;j<4;j++){
      *(pKey+i+4*j)=*(pKey+i+4*j)^*(pKey+i+4*j-1);
    }

  }

}

void generationRoundKey2(word8 *pKey, int numeroRound, word8 *pKeyPrecedente){

  int i, j;

  word8 colonnaTemp[4];

  numeroRound--;

  //calcolo la trasformata della terza colonna
  for(i=0;i<4;i++)
    colonnaTemp[i]=*(pKeyPrecedente + 3*(N_Round+1) + 4*i*(N_Round+1));

  nuovaColonna(&(colonnaTemp[0]), numeroRound);

  //nuova chiave//

  //prima colonna
  for(i=0;i<4;i++)
    *(pKey+4*(N_Round+1)*i)=*(pKeyPrecedente+4*(N_Round+1)*i)^colonnaTemp[i];

  //altre colonne
  for(i=1;i<4;i++){

    for(j=0;j<4;j++){
      *(pKey+i*(N_Round+1)+4*(1+N_Round)*j)=*(pKeyPrecedente+i*(N_Round+1)+4*(1+N_Round)*j)^*(pKey+(i-1)*(1+N_Round)+4*(1+N_Round)*j);
    }

  }

}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*shift rows*/

void shiftRows(word8 *p){

  word8 temp[3];
  int i, j;

  for(i=1;i<4;i++){
    for(j=0;j<i;j++)
      temp[j]=*(p+4*i+j);

    for(j=0;j<(4-i);j++)
      *(p+4*i+j)=*(p+4*i+j+i);

    for(j=(4-i);j<4;j++)
      *(p+4*i+j)=temp[j-4+i];
  }

}

/*inverse shift rows*/

void inverseShiftRows(word8 *p){

  word8 temp[3];
  int i, j;

  for(i=1;i<4;i++){
    for(j=3;j>(3-i);j--)
      temp[j-1]=*(p+4*i+j);

    for(j=3;j>i-1;j--)
      *(p+4*i+j)=*(p+4*i+j-i);

    for(j=0;j<i;j++)
      *(p+4*i+j)=temp[3-i+j];
  }

}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*MixColumn*/

void mixColumn(word8 *p){

  int i, j;
  word8 colonna[4], nuovaColonna[4];

  //separo le colonne e calcolo le nuove
  for(i=0;i<4;i++){

    //prendo la colonna i-sima
    for(j=0;j<4;j++){
      colonna[j]=*(p + i + 4*j);
    }

    //calcolo nuova colonna
    nuovaColonna[0]= multiplicationX(colonna[0]) ^ multiplicationX(colonna[1]) ^ colonna[1] ^ colonna[2] ^ colonna[3];
    nuovaColonna[1]= colonna[0] ^ multiplicationX(colonna[1]) ^ multiplicationX(colonna[2]) ^ colonna[2] ^ colonna[3];
    nuovaColonna[2]= colonna[0] ^ colonna[1] ^ multiplicationX(colonna[2]) ^ multiplicationX(colonna[3]) ^ colonna[3];
    nuovaColonna[3]= multiplicationX(colonna[0]) ^ colonna[0] ^ colonna[1] ^ colonna[2] ^ multiplicationX(colonna[3]);

    //reinserisco colonna
    for(j=0;j<4;j++){
      *(p + i + 4*j)=nuovaColonna[j];
    }

  }

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

/*inverse MixColumn*/
void inverseMixColumn(word8 *p){

  int i, j;
  word8 colonna[4], nuovaColonna[4];

  //separo le colonne e calcolo le nuove
  for(i=0;i<4;i++){

    //prendo la colonna i-sima
    for(j=0;j<4;j++){
      colonna[j]=*(p + i + 4*j);
    }

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
      *(p + i + 4*j)=nuovaColonna[j];
    }

  }

}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*encryption*/

void encryption(word8 initialMessage[][4], word8 initialKey[][4], word8 *ciphertext){

  int i, j;

  //inizializzo lo stato
  unsigned char state[4][4];
  initialization(&(state[0][0]), initialMessage);

  //inizializzo la chiave
  unsigned char key[4][4];
  initialization(&(key[0][0]), initialKey);

  //Initial Round
  addRoundKey(&(state[0][0]), key);

  //Round
  for(i=0; i<N_Round-1; i++){
    generationRoundKey(&(key[0][0]), i);
    byteSubTransformation(&(state[0][0]));
    shiftRows(&(state[0][0]));
    mixColumn(&(state[0][0]));
    addRoundKey(&(state[0][0]), key);

  }

  //Final Round
  generationRoundKey(&(key[0][0]), N_Round-1);
  byteSubTransformation(&(state[0][0]));
  shiftRows(&(state[0][0]));
  addRoundKey(&(state[0][0]), key);

  //salvo messaggio cifrato
  for(i=0; i<4; i++){
    for(j=0; j<4; j++)
      *(ciphertext+j+4*i)=state[i][j];
  }

}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*decryption*/

void decryption(word8 initialMessage[][4], word8 initialKey[][4], word8 *deciphertext){

  int i, j;

  //inizializzo lo stato
  unsigned char state[4][4];
  initialization(&(state[0][0]), initialMessage);

  //calcolo tutte le chiavi//

  unsigned char key[4][4][N_Round+1];
  //chiave iniziale
  initialization2(&(key[0][0][0]), initialKey);

  //chiavi dei vari round
  for(i=1; i<N_Round+1; i++){
    generationRoundKey2(&(key[0][0][i]), i, &(key[0][0][i-1]));
  }


  //Initial Round
  addRoundKey2(&(state[0][0]), key, N_Round);


  //Round
  for(i=N_Round-1; i>0; i--){
    inverseByteSubTransformation(&(state[0][0]));

    inverseShiftRows(&(state[0][0]));

    inverseMixColumn(&(state[0][0]));

    invAddRoundKey(&(state[0][0]), key, i);

  }

  //Final Round
  inverseByteSubTransformation(&(state[0][0]));
  inverseShiftRows(&(state[0][0]));
  addRoundKey2(&(state[0][0]), key, 0);

  for(i=0;i<4;i++){
    for(j=0;j<4;j++)
      *(deciphertext+j+4*i)=state[i][j];
  }

}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

word8 usefulCalculation(word8 a, word8 b, word8 key)
{
    word8 r;

    r = inverseByteTransformation(a ^ key) ^ inverseByteTransformation(b ^ key);

    return r;
}

word8 calcolo(word8 a, int i)
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
                if(calcolo(valore, ciao) == calcolo(test[l], ciao2))
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

    for(j = 0; j < N_Round-1; j++)
        generationRoundKey(&(PreFinalKey[0][0]), j);

    for(i=0;i<4;i++)
    {
        for(k=0;k<4;k++)
            FinalKey[i][k] = initialKey[i][k];
    }
    for(j = 0; j < N_Round; j++)
        generationRoundKey(&(FinalKey[0][0]), j);

    inverseMixColumn(&(PreFinalKey[0][0]));
    inverseShiftRows(&(PreFinalKey[0][0]));

//    printf("We guess all the possibility values of 2 bytes of final key.\n Real values:");
//    printf("position 0,0: %x - position 0,3 right byte: %x\n\n", FinalKey[0][0], FinalKey[0][3]);
//    printf("Byte of the key of the previous round:\n");
//    printf("0,0: 0x%02x - 1,0: 0x%02x - 1,1: 0x%02x - 2,1: 0x%02x\n", PreFinalKey[0][0], PreFinalKey[1][0], PreFinalKey[1][1], PreFinalKey[2][1]);
//    printf("2,2: 0x%02x - 3,2: 0x%02x - 0,3: 0x%02x - 3,3: 0x%02x\n\n", PreFinalKey[2][2], PreFinalKey[3][2], PreFinalKey[0][3], PreFinalKey[3][3]);

    srand(time(NULL));

    rCostante=0x02;
    for(i=1;i<N_Round-1;i++)
        rCostante=multiplicationX(rCostante);

    //printf("const: %x\n\n", rCostante);

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

    printf("ATTACK 4 ROUNDS of AES - EXTENSION AT THE END.\n\n");
    printf("In this attack, we guess 8 bytes of the final key. Then we found 8 byte of the key of the third round.");
    printf("Finally, we guess the remaining the last 3 bytes of the final key and we implement a brute force attack.\n");
    printf("For simplicity, we guess only 2 bytes of the final key, and we check that among the found combinations, the right one is present.\n\n");

    attack4Rounds_3CP(initialKey);

    return 0;
}





