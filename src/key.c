/**************************************
@ filename    : key.c
@ author      : yyrwkk
@ create time : 2024/02/03 23:38:12
@ version     : v1.0.0
**************************************/
#include "key.h"

RoundKey_s roundKeyor;
RoundKey_s roundKeyor_last;

/**
* @ bref: KeyExpansion sbox
*/
const unsigned char sbox[256] = {
  //0    1     2      3     4    5     6     7      8    9     A      B    C     D     E     F
  0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76,
  0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0,
  0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15,
  0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75,
  0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84,
  0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf,
  0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8,
  0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2,
  0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73,
  0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb,
  0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79,
  0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08,
  0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a,
  0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e,
  0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf,
  0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16 
};

/**
* @ bref: const value
* the full form is : 0x8d000000, 0x01000000 ...
* in xor calc, the value ^ 0x00 = value, so skip 0x00
* the format of 128 bit, just use Rcon[1] ~ Rcon[11], the Rcon[0] is not in use
*/
static const unsigned char Rcon[11] = {
  0x8d, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36 
};

/*
* @ bref: calc roundkey
* @ param roundKeyor_last: the roundkey of last round
* @ param rounds : 1-10 , rounds 0 is reserved, which is for init key in logic
*/
RoundKey_s * KeyExpansion(RoundKey_s * roundKeyor_last,int rounds){
    if( rounds < 1 || rounds > 10) {
        printf("the param of rounds is not in [1,10]\r\n");
        return NULL;
    }
    if( roundKeyor_last == NULL ) {
        printf("the input roundKeyor_last is NULL!\r\n");
        return NULL;
    }

    unsigned char roundkey_last_3 [4] = {0};

    // calc roundKey_0
    roundkey_last_3[0] = roundKeyor_last->roundkey_3[1];
    roundkey_last_3[1] = roundKeyor_last->roundkey_3[2];
    roundkey_last_3[2] = roundKeyor_last->roundkey_3[3];
    roundkey_last_3[3] = roundKeyor_last->roundkey_3[0];

    for( unsigned char idx = 0;idx < 4;idx++){
        roundkey_last_3[idx] = sbox[roundkey_last_3[idx]];
    }
    // just roundkey_0[0] need to xor with Rcon, because other is xor with 0x00
    roundkey_last_3[0] ^= Rcon[rounds];
    for( unsigned char idx = 0;idx < 4;idx++){
        roundKeyor.roundkey_0[idx] = roundKeyor_last->roundkey_0[idx] ^ 
                                     roundkey_last_3[idx];
    }

    // calc roundKey_1
    for( unsigned char idx = 0;idx<4;idx++){
        roundKeyor.roundkey_1[idx] = roundKeyor_last->roundkey_1[idx] ^ 
                                     roundKeyor.roundkey_0[idx];
    }

    // calc roundKey_2
    for( unsigned char idx = 0;idx<4;idx++){
        roundKeyor.roundkey_2[idx] = roundKeyor_last->roundkey_2[idx] ^ 
                                     roundKeyor.roundkey_1[idx];
    }

    // calc roundKey_3
    for( unsigned char idx = 0;idx<4;idx++){
        roundKeyor.roundkey_3[idx] = roundKeyor_last->roundkey_3[idx] ^ 
                                     roundKeyor.roundkey_2[idx];
    }

    return &roundKeyor;
}

/**
* @ bref: Update global variable roundKeyor_last
*/
RoundKey_s * UpdateLastRoundKey(RoundKey_s * roundKeyor){
    if( roundKeyor == NULL ) {
        printf("the input roundKeyor is NULL!\r\n");
        return NULL;
    }
    // get roundKey_0
    for( unsigned char idx = 0;idx<4;idx++){
       roundKeyor_last.roundkey_0[idx] = roundKeyor->roundkey_0[idx];
    }

    // get roundKey_1
    for( unsigned char idx = 0;idx<4;idx++){
       roundKeyor_last.roundkey_1[idx] = roundKeyor->roundkey_1[idx];
    }

    // get roundKey_2
    for( unsigned char idx = 0;idx<4;idx++){
       roundKeyor_last.roundkey_2[idx] = roundKeyor->roundkey_2[idx];
    }

    // get roundKey_3
    for( unsigned char idx = 0;idx<4;idx++){
       roundKeyor_last.roundkey_3[idx] = roundKeyor->roundkey_3[idx];
    }

    return &roundKeyor_last;
}

/**
* @ bref: get original roundKey from 128 bit key
*/
RoundKey_s * GetOriginRoundKey(unsigned char * key){
    if( key == NULL ) {
        printf("the input key is NULL!\r\n");
        return NULL;
    }
    unsigned char key_idx = 0;
     // get roundKey_0
    for( unsigned char idx = 0;idx<4;idx++){
       roundKeyor_last.roundkey_0[idx] = key[key_idx++];
    }

    // get roundKey_1
    for( unsigned char idx = 0;idx<4;idx++){
       roundKeyor_last.roundkey_1[idx] = key[key_idx++];
    }

    // get roundKey_2
    for( unsigned char idx = 0;idx<4;idx++){
       roundKeyor_last.roundkey_2[idx] = key[key_idx++];
    }

    // get roundKey_3
    for( unsigned char idx = 0;idx<4;idx++){
       roundKeyor_last.roundkey_3[idx] = key[key_idx++];
    }

    return &roundKeyor_last; 
}

/**
* @ bref: display the value of roundkey variable in hex
*/
void PrintRoundKey(RoundKey_s * roundKeyor){
    if( roundKeyor == NULL ) {
        printf("the input roundKeyor is NULL!\r\n");
        return ;
    }

    printf("--------------------------------------\r\n");
    // print roundKey_0
    for( unsigned char idx = 0;idx<4;idx++){
        printf("%x ",roundKeyor->roundkey_0[idx]);
    }
    printf("\r\n");

    // print roundKey_1
    for( unsigned char idx = 0;idx<4;idx++){
       printf("%x ",roundKeyor->roundkey_1[idx]);
    }
    printf("\r\n");

    // print roundKey_2
    for( unsigned char idx = 0;idx<4;idx++){
       printf("%x ",roundKeyor->roundkey_2[idx]);
    }
    printf("\r\n");

    // print roundKey_3
    for( unsigned char idx = 0;idx<4;idx++){
       printf("%x ",roundKeyor->roundkey_3[idx]);
    }
    printf("\r\n");
    printf("--------------------------------------\r\n");
}