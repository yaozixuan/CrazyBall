#ifndef __EIE3810_KEY_H
#define __EIE3810_KEY_H
#include "stm32f10x.h"

void EIE3810_KEY_Init(void);
u32 EIE3810_read_KEY2(void);
u32 EIE3810_read_KEY1(void);
u32 EIE3810_read_KEY_UP(void);
u32 EIE3810_read_KEY0(void);
void EIE3810_Key1_EXTIInit(void);
void EIE3810_KeyUp_EXTIInit(void);
void EIE3810_Key0_EXTIInit(void);
void EIE3810_Key2_EXTIInit(void);
//void Delay(u32);

#endif
