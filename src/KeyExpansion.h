#ifndef _KEYEXPANSION_H_
#define _KEYEXPANSION_H_

#include <stdio.h>

typedef struct RoundKey_s {
    unsigned char roundkey_0[4];
    unsigned char roundkey_1[4];
    unsigned char roundkey_2[4];
    unsigned char roundkey_3[4];
}RoundKey_s;

RoundKey_s * KeyExpansion(RoundKey_s * roundKeyor_last,int rounds);
RoundKey_s * UpdateLastRoundKey(RoundKey_s * roundKeyor);
RoundKey_s * GetOriginRoundKey(unsigned char * key);
void PrintRoundKey(RoundKey_s * roundKeyor);

#endif