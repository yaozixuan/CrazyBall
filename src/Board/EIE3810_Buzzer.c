#include "stm32f10x.h"
#include "EIE3810_Buzzer.h"
#include "EIE3810_KEY.h"


void EIE3810_Buzzer_Init(void){
	RCC->APB2ENR|=1<<3; //PB8 Buzzer
	GPIOB->CRH &=0xFFFFFFF0;
	GPIOB->CRH |=0x00000003;
}

u32 count2= 0;
	
void EIE3810_toggle_Buzzer(void)
{
	if (EIE3810_read_KEY_UP() == 1)
	{
		if (count2 ==0 )
		{
			count2 = 1;			
			GPIOB->ODR ^= 1<<8;
		}
	}
	else{
		count2=0;
	}
}
