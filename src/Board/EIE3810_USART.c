#include "stm32f10x.h"
#include "stm32f10x_it.h"
#include "EIE3810_LED.h"
#include "EIE3810_USART.h"
#include "EIE3810_TFTLCD.h"
extern u8 Page;
extern u32 buffer;
	
void EIE3810_USART1_Init(u32 pclk1, u32 baudrate)
{
	//USART2
	float temp;
	u16 mantissa;
	u16 fraction;
	temp=(float) (pclk1*1000000)/(baudrate*16);
	mantissa=temp;
	fraction=(temp-mantissa)*16;
	mantissa<<=4;
	mantissa+=fraction;
	RCC->APB2ENR |= 1<<2; //enable GPIOA	
	GPIOA->CRH &= 0xFFFFF00F; //pe10 pe9
	GPIOA->CRH |= 0x000008B0; //PE3 INPUT MODE, PE2 OUTPUT MODE
	RCC->APB2RSTR |= 1<<14; //RESET USART1
	RCC->APB2RSTR &= ~(1<<14); //REVERSE set others
	USART1->BRR=mantissa;//set rate
	USART1->CR1=0x202C; //enable transmitter, enable receiver, USART enable
	USART1->CR1 |= 1<<5; //enable receive interrupt
	USART1->CR1 |= 1<<2; //enable reveiver
}

void EIE3810_USART2_Init(u32 pclk1, u32 baudrate)
{
	//USART2
	float temp;
	u16 mantissa;
	u16 fraction;
	temp=(float) (pclk1*1000000)/(baudrate*16);
	mantissa=temp;
	fraction=(temp-mantissa)*16;
	mantissa<<=4;
	mantissa+=fraction;
	RCC->APB2ENR |= 1<<2; //enable GPIOA	
	GPIOA->CRL &= 0xFFFF00FF; //pe3 pe2
	GPIOA->CRL |= 0x00008B00; //PE3 INPUT MODE, PE2 OUTPUT MODE
	RCC->APB1RSTR |= 1<<17; //RESET USART2
	RCC->APB1RSTR &= ~(1<<17); //REVERSE set others
	USART2->BRR=mantissa;//set rate
	USART2->CR1=0x2008; //enable transmitter, USART enable
}


void USART_print(u8 USARTport, char *st)
{
	u8 i=0;
	while(st[i]!=0x00)
	{
		if (USARTport==1)USART1->DR=st[i];
		if (USARTport==2)USART2->DR=st[i];
		u32 transmitStatus1;
		u32 transmitStatus2;
		if (USARTport==1){
			transmitStatus1=USART1->SR&0x00000080;
			while (transmitStatus1==0){
			transmitStatus1=USART1->SR&0x00000080;
			}
		}
		if (USARTport==2){
			transmitStatus2=USART2->SR&0x00000080;
			while (transmitStatus2==0){
			transmitStatus2=USART2->SR&0x00000080;
			}
		}
		if (i==255)break;
		i++;
	}	
}

void EIE3810_USART1_EXTIInit(void)
{
	NVIC->IP[37] = 0x65; //Set the priority of USART1 to 0x65
	NVIC->ISER[1] |=1<<5; //Enable USART1 global interrupt (interrupt 37)
}

void USART1_IRQHandler(void)
{
	u32 CR1;
	if ((USART1->SR & (1<<5)) && (Page == 3)) //check if received data is ready to be read
	{
		buffer=USART1->DR + 0x30; //read the data value to buffer
		CR1=USART1->CR1;
		if (buffer== '0')
		{
			printString(120, 450, "The random number received is: 0", WHITE, RED);
		}
		if (buffer=='1')
		{
			printString(120, 450, "The random number received is: 1", WHITE, RED);
		}
		if (buffer=='2')
		{
			printString(120, 450, "The random number received is: 2", WHITE, RED);
		}
		if (buffer=='3')
		{
			printString(120, 450, "The random number received is: 3", WHITE, RED);
		}
		if (buffer=='4')
		{
			printString(120, 450, "The random number received is: 4", WHITE, RED);
		}
		if (buffer=='5')
		{
			printString(120, 450, "The random number received is: 5", WHITE, RED);
		}
		if (buffer=='6')
		{
			printString(120, 450, "The random number received is: 6", WHITE, RED);
		}
		if (buffer=='7')
		{
			printString(120, 450, "The random number received is: 7", WHITE, RED);
		}
	
	Delay(20000000);
	EIE3810_TFTLCD_FillRectangle(0, 480, 0, 800, WHITE);
	Page = 10;
	
	
	}
}
