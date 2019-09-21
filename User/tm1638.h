#ifndef __TM1638_H
#define __TM1638_H
#include "common.h"

#define Delay400ns() Delay_us(1)

#define TM1638_DIO PCout(10)
#define TM1638_CLK PCout(11)
#define TM1638_STB PCout(12)

void TM1638_Set(u8 addr, u16 dat);
void TM1638_SetNum(u8 addr, u8 num);
void TM1638_SetNumAndDot(u8 addr, u8 num);
void TM1638_Init();

#endif