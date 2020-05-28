#include "stm32f10x.h"
#include "EIE3810_Clock.h"
#include "EIE3810_LED.h"
#include "EIE3810_KEY.h"
#include "EIE3810_TFTLCD.h"
#include "EIE3810_USART.h"
#include "EIE3810_Buzzer.h"
#include "stm32f10x_it.h"
#include <stdio.h>

void EIE3810_NVIC_SetPriorityGroup(u8 prigroup);
void EIE3810_TIM3_Init(u16 arr, u16 psc);
void EIE3810_TIM4_Init(u16 arr, u16 psc);
void JOYPAD_Init();
void JOYPAD_Init();
void Delay(u32);
void EIE3810_SYSTICK_Init();
u8 task1HeartBeat;
u16 x = 240;
u16 y = 730;	
u32 xdir;
u32 ydir = 1;
u16 x1 = 200;
u16 x2 = 200;
u8 hard = 1;
u8 CPU;
u32 buffer;
u16 Upper = 1;
u16 Lower = 1;
u16 Bounces = 1;
u16 Timer;
char t[10];
char upper[10];
char lower[10];
char bounces[10];
extern u8 count;
extern u8 acce;
u8 button = 0;

#define LED0_PWM_VAL TIM3->CCR2 //Capture/Compare 2 value

u8 Page = 0;

int main(void)
{
	EIE3810_LED_Init();
	EIE3810_clock_tree_init1();
	EIE3810_TFTLCD_Init();
	EIE3810_KEY_Init();
	EIE3810_Key1_EXTIInit();
	EIE3810_Key2_EXTIInit();
	EIE3810_KeyUp_EXTIInit();
	EIE3810_Key0_EXTIInit();
	EIE3810_USART1_Init(72, 14400);
	EIE3810_USART1_EXTIInit();
	EIE3810_SYSTICK_Init();
	EIE3810_TIM3_Init(99,7199); //100Hz 10ms
	EIE3810_Buzzer_Init();
	JOYPAD_Init();
	EIE3810_NVIC_SetPriorityGroup(5);//Set PRIGROUP
	EIE3810_TFTLCD_DrawALL(WHITE);
	Delay(5000000);
	Page = 1;
	if (Page == 1){ //Welcome Page
		printString2412(100, 100, "Welcome to mini Project!", WHITE, BLUE);
		Delay(5000000);
		printString(120, 150, "This is the Final Lab.", WHITE, RED);
		Delay(5000000);
		printString(120, 200, "Are you ready?", WHITE, RED);
		Delay(5000000);
		printString(120, 250, "OK! Let's start.", WHITE, RED);
		Delay(5000000);
		Page = 2;
	}
	if (Page == 2){ //Difficulty Level Page
		EIE3810_TFTLCD_FillRectangle(0, 480, 0, 800, WHITE);
		printString(120, 300, "Please select the difficulty level:", WHITE, RED);
		printString(120, 350, "Easy", WHITE, BLUE);
		printString(120, 400, "Hard", BLUE, WHITE);
		printString(120, 450, "Press KEY0 to enter.", WHITE, RED);
		Delay(5000000);
	}
	
	

	
	while(1){
		if ((Page == 10) & (!CPU)){ //CPU Page
			printString(100, 300, "Please select who you would like to play with:", WHITE, RED);
			printString(120, 350, "Play with another user with JoyPad.", WHITE, BLUE);
			printString(120, 400, "Play with CPU.", BLUE, WHITE);
			printString(120, 450, "Press KEY0 to enter.", WHITE, RED);
			Delay(5000000);
		}
		
		if (Page == 6){ //Stop Page
			printString(120, 500, "You are in STOP mode", WHITE, BLUE);
			EIE3810_turn_on_LED0();
			count = 1;
			while(Page == 6);
		}
		
		if (Page == 5){ //End Page
			Delay(30000000);
			EIE3810_TFTLCD_FillRectangle(0, 480, 0, 800, WHITE);
			EIE3810_TFTLCD_DrawCircle(240,400, 240, 1, RED);
			Delay(10000000);
			EIE3810_TFTLCD_DrawCircle(240,400, 180, 1, YELLOW);
			Delay(10000000);
			EIE3810_TFTLCD_DrawCircle(240,400, 120, 1, BLUE);
			Delay(10000000);
			EIE3810_TFTLCD_DrawCircle(240,400, 60, 1, GREEN);
			Delay(10000000);
		}
		
		if (Page == 4){ //Game Page
			count = 0;
			EIE3810_TIM4_Init(9999,7199); // timer start
			if (!xdir){ // control initial direction
				if (buffer <= 51){
					xdir = 1;
				}
				else{
					xdir = -1;
				}
			}
			while ((task1HeartBeat >= 2) && (Page == 4)){
				EIE3810_TFTLCD_DrawCircle(x,y, 10, 1, WHITE);

				if ((10 <= x) && (x <= 470)){ //in
					if (xdir == 1){ //right
						x = x + 2 * hard;
					}
					if (xdir == -1){ //left
						x = x - 2 * hard;
					}
				}
				else if (x > 470){ //out
					xdir = -1;
					x = x - 5;
					GPIOB->ODR ^= 1<<8;
					Delay(50000);
					GPIOB->ODR ^= 1<<8;
					sprintf(bounces,"%d",Bounces);
					printString(9*18,700 - 36, bounces ,WHITE,WHITE);
					Bounces ++;
					printString(9*18,700 - 36, bounces ,RED,WHITE);
				}
				else if (x < 10){ //out
					xdir = 1;
					x = x + 5;
					GPIOB->ODR ^= 1<<8;
					Delay(50000);
					GPIOB->ODR ^= 1<<8;
					sprintf(bounces,"%d",Bounces);
					printString(9*18,700 - 36, bounces ,WHITE,WHITE);
					Bounces ++;
					printString(9*18,700 - 36, bounces ,RED,WHITE);
				}
				
				if ((20<= y) && (y <= 780)){
					if (ydir == -1){
						if (buffer <= 51){
							y = y + ((buffer - 48)*2 +1) * hard;
						}
						else{
							y = y + ((buffer - 48 - 4)*2 + 1) * hard;
						}
					}
					if (ydir == 1){
						if (buffer <= 51){
							y = y - ((buffer - 48)*2 +1) * hard;
						}
						else{
							y = y - ((buffer - 48 - 4)*2 + 1) * hard;
						}
					}
				}
				
				else if (y > 780){
					if ((x >= x2-5) && (x <= x2+85)){
						y = y - 6;
						ydir = 1;
						GPIOB->ODR ^= 1<<8;
						Delay(50000);
						GPIOB->ODR ^= 1<<8;
						sprintf(bounces,"%d",Bounces);
						printString(9*18,700 - 36, bounces ,WHITE,WHITE);
						Bounces ++;
						printString(9*18,700 - 36, bounces ,RED,WHITE);
						sprintf(lower,"%d",Lower);
						printString(9*18,700+36, lower ,WHITE,WHITE);
						Lower ++;
						printString(9*18,700+36, lower ,RED,WHITE);
					}
					else{
						Delay(5000000);
						//EIE3810_TFTLCD_FillRectangle(0, 480, 0, 800, WHITE);
						printString2412(0x64+125,0x64,"Game Over",WHITE,BLACK);
						printString2412(0x64+125,0x64+50,"Upper Player Wins!",WHITE,BLACK);
						if ((x > 210) && (x <= 336)){
							printString2412(x,740,"Death review",WHITE,BLACK);
						}
						else if (x > 336){
							printString2412(336,740,"Death review",WHITE,BLACK);
						}
						else{
							printString2412(190,740,"Death review",WHITE,BLACK);
						}
						Page = 5;
					}
				}
				else if (y < 20){
					if ((x >= x1-5) && (x <= x1+85)){
						y = y + 6;
						ydir = -1;
						GPIOB->ODR ^= 1<<8;
						Delay(50000);
						GPIOB->ODR ^= 1<<8;
						sprintf(bounces,"%d",Bounces);
						printString(9*18,700 - 36, bounces ,WHITE,WHITE);
						Bounces ++;
						printString(9*18,700 - 36, bounces ,RED,WHITE);
						sprintf(upper,"%d",Upper);
						printString(9*18,700, upper ,WHITE,WHITE);
						Upper ++;
						printString(9*18,700, upper ,RED,WHITE);
					}
					else{
						Delay(5000000);
						//EIE3810_TFTLCD_FillRectangle(0, 480, 0, 800, WHITE);
						printString2412(0x64+125,0x64,"Game Over",WHITE,BLACK);
						printString2412(0x64+125,0x64+50,"Lower Player Wins!",WHITE,BLACK);
						if (x <= 336){
							printString2412(x,36,"Death review",WHITE,BLACK);
						}
						else if (x > 336){
							printString2412(336,36,"Death review",WHITE,BLACK);
						}
						Page = 5;
					}
				}
				
				if (Bounces%6 == 0){
					EIE3810_TFTLCD_DrawCircle(x,y, 10, 1, RED);
				}
				else if (Bounces%6 == 1){
					EIE3810_TFTLCD_DrawCircle(x,y, 10, 1, YELLOW);
				}
				else if (Bounces%6 == 2){
					EIE3810_TFTLCD_DrawCircle(x,y, 10, 1, BLUE);
				}
				else if (Bounces%6 == 3){
					EIE3810_TFTLCD_DrawCircle(x,y, 10, 1, GREEN);
				}
				else if (Bounces%6 == 4){
					EIE3810_TFTLCD_DrawCircle(x,y, 10, 1, BLACK);
				}
				else if (Bounces%6 == 5){
					EIE3810_TFTLCD_DrawCircle(x,y, 10, 1, WHITE);
				}
				
				EIE3810_TFTLCD_FillRectangle(x2, 80, 795, 5, BLACK);
				
				if (CPU){ // Play with CPU
					EIE3810_TFTLCD_FillRectangle(x1, 80, 0, 5, WHITE);
				if ((x >= 40) & (x <= 440)){
					x1 = x - 40;
				}
				else if (x < 40){
					x1 = 0;
				}
				else{
					x1 = 400;
				}	
					EIE3810_TFTLCD_FillRectangle(x1, 80, 0, 5, BLACK);
				}
				
				task1HeartBeat = 0;
			}
		}
	}
}


	
void JOYPAD_Init(void)
{
	RCC->APB2ENR|=1<<3;//Enable GPIOB
	RCC->APB2ENR|=1<<5;//Enable GPIOD
	GPIOB->CRH&=0xFFFF00FF;//Enable PB10 and PB11
	GPIOB->CRH|=0x00003800;// PB10 as input mode, PB11 as output mode
	GPIOB->ODR|=3<<10; //PB10 as input pull up mode, set PB11
	GPIOD->CRL&=0xFFFF0FFF;
	GPIOD->CRL|=0x00003000;//PD3 as output mode
	GPIOD->ODR|=1<<3; //set PD3
}

void JOYPAD_Delay(u16 t)
{
	while (t--);
}

u8 JOYPAD_Read(void)
{
	vu8 temp=0;
	u8 t;
	GPIOB->BSRR |= 1<<11;//Set PB11
	Delay(80);
	GPIOB->BSRR|=1<<27;//Reset PB11, transmir low signal to joypad
	for (t=0;t<8;t++)
	{
		temp>>=1;//Move temp to the right 1 bit
		if((((GPIOB->IDR)>>10)&0x01)==0)temp|=0x80;//read from the PB10
		GPIOD->BSRR |=(1<<3);//Set PD3, send 8 pulse on PD3
		Delay(80);
		GPIOD->BSRR |=  (1<<19);//Reset PD3
		Delay(80);
	}
	return temp;
}


void EIE3810_TIM3_Init(u16 arr, u16 psc)
{
	//TIM3
	RCC->APB1ENR|=1<<1;//Enable TIM3
	TIM3->ARR=arr;//Set auto reload register
	TIM3->PSC=psc;//Set pre-scalar
	TIM3->DIER|=1<<0;//Update interrupt enable
	TIM3->CR1|=0x01;//Counter enable
	NVIC->IP[29]=0x45;//Set priority of TIM3 to 0x45
	NVIC->ISER[0]=(1<<29);//Enable TIM3 global interrupt (interrupt 29)
}


void TIM3_IRQHandler(void)
{
	if(TIM3->SR&1<<0)
	{
		
		u8 JoyPadInput;
		JoyPadInput=JOYPAD_Read();
		if (JoyPadInput)
		{
			u8 button1=JoyPadInput & 0x01;
			u8 button2=JoyPadInput & 0x02;
			u8 button3=JoyPadInput & 0x04;
			u8 button4=JoyPadInput & 0x08;
			u8 button5=JoyPadInput & 0x10;
			u8 button6=JoyPadInput & 0x20;
			u8 button7=JoyPadInput & 0x40;
			u8 button8=JoyPadInput & 0x80;
//			if (button1){
//				EIE3810_TFTLCD_ShowChar(0x64+300,0x64,'A',WHITE,BLACK);
//			}	
//			if (button2){
//				EIE3810_TFTLCD_ShowChar(0x64+275,0x64,'B',WHITE,BLACK);
//			}	
			if (button3){
				button++;
				if ((button >2) && (Page == 4)){ //JoyPad
					if (acce == 0){
						printString(120, 350, "Acceleration.", WHITE, BLUE);
						hard = 2*hard;
						acce = 1;
					}
				}
			}	
			else if (button4){
				button++;
				if ((button >2) && (Page == 4) && (count == 0)){
					Page = 6; // Go to Stop Page
					EIE3810_turn_off_LED1();
					button = 0;
				}
				if ((button >2) && (Page == 6) && (count == 1)){
					Page = 4; // Go back to Game Page
					EIE3810_turn_on_LED1();
					printString(120, 500, "You are in STOP mode", WHITE, WHITE); //Clear
					button = 0;
				}
			}	
			else if (button5>>4){
				button++;
				if ((button > 2) && (Page == 2)){
					printString(120, 350, "Easy", WHITE, BLUE);
					printString(120, 400, "Hard", BLUE, WHITE);
					hard = 1;
					button = 0;
				}
			}
			else if (button6>>5){
				button++;
				if ((button > 2)  && (Page == 2)){
					printString(120, 350, "Easy", BLUE, WHITE);
					printString(120, 400, "Hard", WHITE, BLUE);
					hard = 2;
					button = 0;
				}
			}
			
			else if ((button7>>6) && (Page == 4)){
				EIE3810_TFTLCD_FillRectangle(x1, 80, 0, 5, WHITE);
				if (x1 > 0){
					x1--;
				}
				EIE3810_TFTLCD_FillRectangle(x1, 80, 0, 5, BLACK);
			}	
			else if ((button8>>7) && (Page == 4)){
				EIE3810_TFTLCD_FillRectangle(x1, 80, 0, 5, WHITE);
				if (x1 <400){
					x1++;
				}
				EIE3810_TFTLCD_FillRectangle(x1, 80, 0, 5, BLACK);
			}
			else{
				count = 0;
			}
		}
	}
	TIM3->SR &=~(1<<0);
}

void EIE3810_TIM4_Init(u16 arr, u16 psc){
    RCC->APB1ENR |=1<<2;     //Enable TIM4
    TIM4->ARR = arr;         //Set auto-reload register.
    TIM4->PSC = psc;         //Set Prescaler.
    TIM4->DIER|=1<<0;         //Interrupt enable register, update interrupt enabled.
    TIM4->CR1 |=0x01;         //Control register, enable counter.
    NVIC->IP[30] = 0x45;     //Set priority, to be 0x45.
    NVIC ->ISER[0] = (1<<30);//Set interrupt Set-Enable register. for #30.
}

void TIM4_IRQHandler(void){
    if ((TIM4->SR & 1<<0) && (Page == 4)){   //Update interrupt pending
		printString(0,700 - 36,"Number of Bounces:", RED, WHITE);
		printString(9*18,700 - 36, bounces ,RED,WHITE);
		printString(0,700,"Upper Player Score:", RED, WHITE);
		printString(9*18,700, upper ,RED,WHITE);
		printString(0,700+36,"Lower Player Score:", RED, WHITE);
		printString(9*18,700+36, lower ,RED,WHITE);
		printString(0,700+72,"Time: ", RED, WHITE);
		printString(9*18,700+72,t,WHITE,WHITE);
		Timer ++;
		sprintf(t,"%d",Timer);
		printString(0,700+72,"Time: ", RED, WHITE);
		printString(9*18,700+72,t,RED,WHITE);
		if (acce == 1){
			printString(120, 350, "Acceleration.", WHITE, BLUE);
		}
    }
    TIM4->SR &= ~(1<<0);    //Clear the update.
}

void EIE3810_NVIC_SetPriorityGroup(u8 prigroup)
{
	u32 temp1, temp2;
	temp2 = prigroup & 0x00000007;
	temp2 <<=8;
	temp1 = SCB->AIRCR;
	temp1 &= 0x0000F8FF;
	temp1 |=0x05FA0000;
	temp1 |= temp2;
	SCB->AIRCR = temp1;
}

void EIE3810_SYSTICK_Init()
{
	//SYSTICK
	SysTick->CTRL = 0;//Clear SysTick->CTRL setting
	SysTick->LOAD = 719999;//72000000/(719999+1)=100Hz
	SysTick->CTRL = 0x7;//CLKSOURCE=0, FLCK; TICKINT=1, counting down to 0 pends SysTick handler; ENABLE=1, counter enable
}

void Delay(u32 count) //Use looping for delay
{
	u32 i;
	for (i=0;i<count;i++);
}
