#include "stm32f10x.h"
#include "EIE3810_KEY.h"
#include "EIE3810_TFTLCD.h"
#include "EIE3810_LED.h"

extern u8 Page;
extern u16 x2;
extern u8 hard;
extern u8 CPU;
extern u8 xdir;

u8 count = 0;
u8 acce = 0;

void EIE3810_KEY_Init(void)
{
	RCC->APB2ENR|=1<<6; //PE2 KEY2 (LEFT)
	GPIOE->CRL &=0xFFFFF0FF;
	GPIOE->CRL |=0x00000800;
	GPIOE->ODR =1<<2;
	RCC->APB2ENR|=1<<6; //PE3 KEY1 (DOWN)
	GPIOE->CRL &=0xFFFF0FFF;
	GPIOE->CRL |=0x00008000;
	GPIOE->ODR |=1<<3;
	RCC->APB2ENR|=1<<2; //PA0 KEY_UP
	GPIOA->CRL &=0xFFFFFFF0;
	GPIOA->CRL |=0x00000008;
	GPIOA->ODR |=0<<8;
	RCC->APB2ENR|=1<<4; //PE2 KEY0 (RIGHT)
	GPIOE->CRL &=0xFFF0FFFF;
	GPIOE->CRL |=0x00080000;
	GPIOE->ODR =1<<2;
}

void EIE3810_Key1_EXTIInit(void)
{
	RCC->APB2ENR |= 1<<6;//Enable GPIOE
	GPIOE->CRL &= 0xFFFF0FFF;//input with pull-up/pull-down
	GPIOE->CRL |= 0x00008000;
	GPIOE->ODR |= 1<<3;
	RCC->APB2ENR |= 0x01;//AFIO enable
	AFIO->EXTICR[0] &= 0xFFFF0FFF;//set external interrupt configuration register for PE3
	AFIO->EXTICR[0] |= 0x00004000;
	EXTI->IMR |= 1<<3;//input request from line3 is not masked
	EXTI->RTSR |= 1<<3;//falling/rising trigger selection register
	NVIC->IP[9] = 0x35;//Set the priority of Key1 to 0x65
	NVIC->ISER[0] |= (1<<9);//Enable EXTI3 (Interrupt 9)
}

void EXTI3_IRQHandler(void)
{
	if (EIE3810_read_KEY1() & (Page == 2)){
		printString(120, 350, "Easy", BLUE, WHITE);
		printString(120, 400, "Hard", WHITE, BLUE);
		hard = 2;
	}
	if (EIE3810_read_KEY1() & (Page == 10)){ //CPU
		printString(120, 350, "Play with another user with JoyPad.", BLUE, WHITE);
		printString(120, 400, "Play with CPU.", WHITE, BLUE);
		EIE3810_turn_on_LED1();
		CPU = 1;
	}
	if (EIE3810_read_KEY1() && (Page == 4) && (count == 0)){
		Page = 6; // Go to Stop Page
		EIE3810_turn_off_LED1();
		//printString(120, 400, "STOP!", WHITE, BLUE);
		//printString(120, 450, "START!", WHITE, WHITE);
	}
	if (EIE3810_read_KEY1() && (Page == 6) && (count == 1)){
		Page = 4; // Go back to Game Page
		EIE3810_turn_on_LED1();
		printString(120, 500, "You are in STOP mode", WHITE, WHITE); //Clear
		//printString(120, 450, "START!", WHITE, BLUE);
		//printString(120, 400, "STOP!", WHITE, WHITE);
	}
	EXTI->PR = 1<<3;//pending register
}

void EIE3810_KeyUp_EXTIInit(void)
{
	RCC->APB2ENR |= 1<<2;//Enanle GPIOA
	GPIOA->CRL &= 0xFFFFFFF0;//input with pull-up/pull-down
	GPIOA->CRL |= 0x00000008;
	GPIOA->ODR &= 0xE;
	RCC->APB2ENR |= 0x01;//AFIO enable
	AFIO->EXTICR[0] &= 0xFFFFFFF0;//set external interrupt configuration register for PA0
	//AFIO->EXTICR[0] |= 0x00000400;
	EXTI->IMR |= 1;//input request from line0 is not masked
	EXTI->RTSR |= 1;//falling/rising trigger selection register
	NVIC->IP[6] = 0x75;//Set the priority of KeyUp to 0x75
	NVIC->ISER[0] |= (1<<6);//Enable EXTI0 (Interrupt 6)
}

void EXTI0_IRQHandler(void)
{
	if (EIE3810_read_KEY_UP() & (Page == 2)){ //Easy
		printString(120, 350, "Easy", WHITE, BLUE);
		printString(120, 400, "Hard", BLUE, WHITE);
		hard = 1;
	}
	if (EIE3810_read_KEY_UP() & (Page == 10)){ //JoyPad
		printString(120, 350, "Play with another user with JoyPad.", WHITE, BLUE);
		printString(120, 400, "Play with CPU.", BLUE, WHITE);
		CPU = 0;
	}
	if (EIE3810_read_KEY_UP() & (Page == 4)){ //JoyPad
		if (acce == 0){
			printString(120, 350, "Acceleration.", WHITE, BLUE);
			hard = 2*hard;
			acce = 1;
		}
//		if (acce == 1){
//			printString(120, 350, "Deceleration.", WHITE, BLUE);
//			hard = 0.5*hard;
//			acce = 0;
//		}
	}
	EXTI->PR = 1;//pending register
}

void EIE3810_Key0_EXTIInit(void)
{
	RCC->APB2ENR |= 1<<6;//Enable GPIOE
	GPIOE->CRL &= 0xFFF0FFFF;//input with pull-up/pull-down
	GPIOE->CRL |= 0x00080000;
	GPIOE->ODR |= 1<<4;
	RCC->APB2ENR |= 0x01;//AFIO enable
	AFIO->EXTICR[1] &= 0xFFFFFFF0;//set external interrupt configuration register for PE4
	AFIO->EXTICR[1] |= 0x00000004;
	EXTI->IMR |= 1<<4;//input request from line4 is not masked
	EXTI->RTSR |= 1<<4;//falling/rising trigger selection register
	NVIC->IP[10] = 0x65;//Set the priority of Key0 to 0x65
	NVIC->ISER[0] |= (1<<10);//Enable EXTI4 (Interrupt 10)
}

void EXTI4_IRQHandler(void)
{
	if (EIE3810_read_KEY0() & (Page == 2)){
		EIE3810_TFTLCD_FillRectangle(0, 480, 0, 800, WHITE);
		printString2412(50, 400, "Use USART for a random direction.", WHITE, RED);
		Page = 3;
	}
	if (EIE3810_read_KEY0() & (Page == 10)){
		EIE3810_TFTLCD_FillRectangle(0, 480, 0, 800, WHITE);
		if (CPU){
			printString2412(50, 400, "You are playing with CPU.", WHITE, RED);
		}
		else{
			printString(50, 400, "You are playing with another user with JoyPad.", WHITE, RED);
		}
		
		Delay(10000000);
		EIE3810_TFTLCD_FillRectangle(0, 480, 0, 800, WHITE);
		EIE3810_TFTLCD_SevenSegment(202, 330, 3, BLUE);
		Delay(10288066);
		EIE3810_TFTLCD_Clear(WHITE);
		EIE3810_TFTLCD_SevenSegment(202, 330, 2, BLUE);
		Delay(10288066);
		EIE3810_TFTLCD_Clear(WHITE);
		EIE3810_TFTLCD_SevenSegment(202, 330, 1, BLUE);
		Delay(10288066);
		EIE3810_TFTLCD_Clear(WHITE);
		EIE3810_TFTLCD_SevenSegment(202, 330, 0, BLUE);
		Delay(10288066);
		EIE3810_TFTLCD_Clear(WHITE);
	
		EIE3810_TFTLCD_FillRectangle(200, 80, 795, 5, BLACK);
		EIE3810_TFTLCD_FillRectangle(200, 80, 0, 5, BLACK);
		Page = 4;
	}
	
	if (EIE3810_read_KEY0() & (Page == 4)){
		EIE3810_TFTLCD_FillRectangle(x2, 80, 795, 5, WHITE);
		if (x2 < 400){
			x2 = x2 + 25;
		}
		EIE3810_TFTLCD_FillRectangle(x2, 80, 795, 5, BLACK);
	}
	EXTI->PR = 1<<4;//pending register
}

void EIE3810_Key2_EXTIInit(void)
{
	RCC->APB2ENR |= 1<<6;//Enable GPIOE
	GPIOE->CRL &= 0xFFFFF0FF;//input with pull-up/pull-down
	GPIOE->CRL |= 0x00000800;
	GPIOE->ODR |= 1<<2;
	RCC->APB2ENR |= 0x01;//AFIO enable
	AFIO->EXTICR[0] &= 0xFFFFF0FF;//set external interrupt configuration register for PE2
	AFIO->EXTICR[0] |= 0x00000400;
	EXTI->IMR |= 1<<2;//input request from line2 is not masked
	EXTI->RTSR |= 1<<2;//falling/rising trigger selection register
	NVIC->IP[8] = 0x65;//Set the priority of Key2 to 0x65
	NVIC->ISER[0] |= (1<<8);//Enable EXTI2 (Interrupt 8)
}

void EXTI2_IRQHandler(void)
{
	if ((EIE3810_read_KEY2()) & (Page == 4)){
		EIE3810_TFTLCD_FillRectangle(x2, 80, 795, 5, WHITE);
		if (x2 > 0){
			x2 = x2 - 25;
		}
		EIE3810_TFTLCD_FillRectangle(x2, 80, 795, 5, BLACK);
	}
	EXTI->PR = 1<<2;//pending register
}

u32 EIE3810_read_KEY2(void)
{
	return ((GPIOE->IDR>>2) & 0x00000001);
}

u32 EIE3810_read_KEY1(void)
{
	return (( GPIOE->IDR>>3) & 0x00000001);
}

u32 EIE3810_read_KEY_UP(void)
{
	return ((GPIOA->IDR) &= 0x00000001);
}

u32 EIE3810_read_KEY0(void)
{
	return ((GPIOE->IDR>>4) & 0x00000001);
}

