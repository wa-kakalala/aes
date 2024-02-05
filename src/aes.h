/**************************************
@ filename    : aes.h
@ author      : yyrwkk
@ create time : 2024/02/04 00:47:59
@ version     : v1.0.0
**************************************/
#ifndef _AES_H_
#define _AES_H_
#include "key.h"

unsigned char * SubBytes(unsigned char * data);
unsigned char * AddRoundKey(unsigned char * data, RoundKey_s* roundKey)
unsigned char * ShiftRows(unsigned char * data);
#endif