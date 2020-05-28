#include "stm32f10x.h"
#include "EIE3810_Clock.h"

void EIE3810_clock_tree_init1(void)
{
	u8 PLL=7;
	u8 temp=0;
	RCC->CR |= 0x00010000; //turn on HSE clock (HSEON 1)
	while(!((RCC->CR>>17)&0x1));//check if the HSE clock is ready (not ready do nothing)
	RCC->CFGR &= 0xFFFDFFFF; //HSE clock not divided (ensure bit17 PLLXTPRE = 0)
	RCC->CFGR |= 1<<16; //HSE oscillator clock selected as PLL input clock (PLLSRC 1)
	RCC->CFGR |= PLL<<18; //PLL input clock x9, so PLLCLK is 72MHz (PLLMUL 0111)
	RCC->CR |=0x01000000;//turn on PLL (PLLON 1)
	while(!(RCC->CR>>25));//check if PLL is locked (not locked do nothing)
	RCC->CFGR &=0xFFFFFFFE;//PLL selected as system clock (ensure SW = x0)
	RCC->CFGR |=0x00000002;//ensure SW = 1x => SW = 10
	while(temp !=0x02) //Check which clock source is used as system clock
	{
		temp=RCC->CFGR>>2;
		temp &= 0x03; //SWS &= 0011
	}	
	RCC->CFGR &= 0xFFFFCF0F;//SYSCLK not divided (HPRE 0000)
	RCC->CFGR |= 0x00000000;//HCLK not divided (PPRE1 0xx), not output yet
	FLASH->ACR = 0x32;//Set FLASH with 2 wait states	
	RCC->APB2ENR |= 1<<14; //USART1 clock enable,output 72MHz of PCLK2 for USART1
}

void EIE3810_clock_tree_init2(void)
{
	u8 PLL=7;
	u8 temp=0;
	RCC->CR |= 0x00010000; //turn on HSE clock (HSEON 1)
	while(!((RCC->CR>>17)&0x1));//check if the HSE clock is ready (not ready do nothing)
	RCC->CFGR &= 0xFFFDFFFF; //HSE clock not divided (ensure bit17 PLLXTPRE = 0)
	RCC->CFGR |= 1<<16; //HSE oscillator clock selected as PLL input clock (PLLSRC 1)
	RCC->CFGR |= PLL<<18; //PLL input clock x9, so PLLCLK is 72MHz (PLLMUL 0111)
	RCC->CR |=0x01000000;//turn on PLL (PLLON 1)
	while(!(RCC->CR>>25));//check if PLL is locked (not locked do nothing)
	RCC->CFGR &=0xFFFFFFFE;//PLL selected as system clock (ensure SW = x0)
	RCC->CFGR |=0x00000002;//ensure SW = 1x => SW = 10
	while(temp !=0x02) //Check which clock source is used as system clock
	{
		temp=RCC->CFGR>>2;
		temp &= 0x03; //SWS &= 0011
	}	
	RCC->CFGR &= 0xFFFFFC0F;//SYSCLK not divided (HPRE 0000)
	RCC->CFGR |= 0x00000400;//HCLK divided by 2 (PPRE1 100), not output yet
	FLASH->ACR = 0x32;//Set FLASH with 2 wait states	
	RCC->APB1ENR |= 1<<17; //USART2 clock enable,output 36MHz of PCLK1 for USART2
}
