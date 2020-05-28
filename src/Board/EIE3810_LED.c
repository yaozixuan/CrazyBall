#include "stm32f10x.h"
#include "EIE3810_LED.h"


void EIE3810_LED_Init(void)
{
	RCC->APB2ENR|=1<<3; //PB5 LED0
	GPIOB->CRL &=0xFF0FFFFF;
	GPIOB->CRL |=0x00300000;
	RCC->APB2ENR|=1<<6; //PE5 LED1
	GPIOE->CRL &=0xFF0FFFFF;
	GPIOE->CRL |=0x00300000;
	
	EIE3810_turn_off_LED0();
	EIE3810_turn_off_LED1();
}

void EIE3810_turn_on_LED0(void)
{
	GPIOB->BRR = 1<<5;
}

void EIE3810_turn_off_LED0(void)
{
	GPIOB->BSRR = 1<<5;
}

void EIE3810_turn_on_LED1(void)
{
	GPIOE->BRR = 1<<5;
}

void EIE3810_turn_off_LED1(void)
{
	GPIOE->BSRR = 1<<5;
}
