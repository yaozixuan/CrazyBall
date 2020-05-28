#ifndef EIE3810_USART_H
#define EIE3810_USART_H
#include "stm32f10x.h"
void EIE3810_USART1_Init(u32 pclk1, u32 baudrate);
void EIE3810_USART2_Init(u32 pclk1, u32 baudrate);
void USART_print(u8 USARTport, char *st);
void EIE3810_USART1_EXTIInit(void);

#endif
