/**************************************
@ filename    : aes.c
@ author      : yyrwkk
@ create time : 2024/02/04 00:48:36
@ version     : v1.0.0
**************************************/
#include "aes.h"

#define BYTES_LEN 16

/**
* @ bref: substitute bytes
*/
unsigned char * SubBytes(unsigned char * data){
    for(unsigned char idx = 0;idx<BYTES_LEN;idx++){
        data[idx] = sbox[idx];
    }
    return data;
}

/**
* @ bref: data xor roundkey
*/
unsigned char * AddRoundKey(unsigned char * data, RoundKey_s* roundKey){
    unsigned char* key = (unsigned char*)roundKey;
    for(unsigned char idx = 0;idx<BYTES_LEN;idx++){
        data[idx] = data[idx] ^key[idx] ;
    }
    return data;
}

/**
* @ bref: Left ShiftRows
*  row 0 -> 0
*  row 1 -> 1
*  row 2 -> 2
*  row 3 -> 3
*/
unsigned char * ShiftRows(unsigned char * data){
    unsigned char temp;
    temp = data[1];
    data[1] = data[5]; data[5] = data[9]; data[9]=data[13]; data[13]=temp;

    temp = data[2];
    data[2] = data[10];data[10]=temp; 
    temp = data[6];
    data[6] = data[14];  data[14]=temp;

    temp = data[3];
    data[3] = data[15]; data[15]=data[11]; data[11] =data[7]; data[7] = temp;

    return data;
}

unsigned char * MixColumns(unsigned char * data){
   
}

