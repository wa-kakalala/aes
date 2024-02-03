/**************************************
@ filename    : main.c
@ author      : yyrwkk
@ create time : 2024/02/03 23:38:22
@ version     : v1.0.0
**************************************/
#include "key.h"

int main(){
    unsigned char key[16] = {0x2b,0x7e,0x15,0x16,0x28,0xae,0xd2,0xa6,0xab,0xf7,0x15,0x88,0x09,0xcf,0x4f,0x3c};
    RoundKey_s * originRoundKey =  GetOriginRoundKey(key);
    RoundKey_s * roundKeyor;
    for( int i =0;i<10;i++){
        roundKeyor = KeyExpansion(originRoundKey,i+1);
        originRoundKey = UpdateLastRoundKey(roundKeyor);
    }
    return 0;
}