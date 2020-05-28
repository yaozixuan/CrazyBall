# ARM_bouncing_ball_game

## Experiment 1
### Design

	First of all, we set Page to “0”, where necessary initializations are done for the program. SYSTICK is initialized for the body part of the game, and TIM3 is initialized for JOYPAD use only. Then, TFTLCD and Buzzer are initialized, Keys and their interrupts are initialized, as well as USART1 and USART1 interrupt.

```
u8 Page = 0;
int main(void)
{
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
```
Figure 1. Page 0: Initialization

	Then, we enter welcome page, print the welcome sentences and enter the next stage.
```
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
```
Figure 2. Page 1: Welcome Page

	We enter Difficulty Level Page, print the selecting sentences. 
```
if (Page == 2){ //Difficulty Level Page
    EIE3810_TFTLCD_FillRectangle(0, 480, 0, 800, WHITE);
    printString(120, 300, "Please select the difficulty level:", WHITE, RED);
    printString(120, 350, "Easy", WHITE, BLUE);
    printString(120, 400, "Hard", BLUE, WHITE);
    printString(120, 450, "Press KEY0 to enter.", WHITE, RED);
    Delay(5000000);
}
```
Figure 3. Page 2: Difficulty Level Page

	On this page, KEY_UP and KEY1 interrupts are used to select easy or hard level. An extern universal variant “hard” is exported to the main program to indicate the difficulty level. “hard = 1” implies easy mode, “hard = 2” implies hard mode.
```
void EXTI0_IRQHandler(void)
{
    if (EIE3810_read_KEY_UP() & (Page == 2)){ //Easy
        printString(120, 350, "Easy", WHITE, BLUE);
        printString(120, 400, "Hard", BLUE, WHITE);
        hard = 1;
    }
Figure 4. KEY_UP interrupt exception in Page 2 to select easy mode
void EXTI3_IRQHandler(void)
{
    if (EIE3810_read_KEY1() & (Page == 2)){
        printString(120, 350, "Easy", BLUE, WHITE);
        printString(120, 400, "Hard", WHITE, BLUE);
        hard = 2;
    }
```
Figure 5. KEY1 interrupt exception in Page 2 to select hard mode

	We enter KEY0 interrupt. Inside KEY0 interrupt exception, we ask the user to send a random direction from USART1 and enter the next page: USART Page.
```
void EXTI4_IRQHandler(void)
{
    if (EIE3810_read_KEY0() & (Page == 2)){
        EIE3810_TFTLCD_FillRectangle(0, 480, 0, 800, WHITE);
        printString2412(50, 400, "Use USART for a random direction.", WHITE, RED);
        Page = 3;
    }
```
Figure 6. KEY0 interrupt exception in Page 2 and enter USART Page

	Inside USART1 exception, the transmitted number is loaded to buffer, which is an extern universal variant exported to the main program to indicate the initial direction of the ball. A sentence indicating the number is printed to the LCD and we enter Game Page.
```
void USART1_IRQHandler(void)
{
    u32 CR1;
    if ((USART1->SR & (1<<5)) && (Page == 3)) //check if received data is ready to be read
    {
        buffer=USART1->DR + 0x30; //read the data value to buffer
        CR1=USART1->CR1;
        if (buffer== '0'{
            printString(120, 450, "The random number received is: 0", WHITE, RED);
        }
        if (buffer=='1'){
            printString(120, 450, "The random number received is: 1", WHITE, RED);
        }
        if (buffer=='2'){
            printString(120, 450, "The random number received is: 2", WHITE, RED);
        }
        if (buffer=='3'){
            printString(120, 450, "The random number received is: 3", WHITE, RED);
        }
        if (buffer=='4'){
            printString(120, 450, "The random number received is: 4", WHITE, RED);
        }
        if (buffer=='5'){
            printString(120, 450, "The random number received is: 5", WHITE, RED);
        }
        if (buffer=='6'){
            printString(120, 450, "The random number received is: 6", WHITE, RED);
        }
        if (buffer=='7'){
            printString(120, 450, "The random number received is: 7", WHITE, RED);
        }
    
    Delay(20000000);
    EIE3810_TFTLCD_FillRectangle(0, 480, 0, 800, WHITE);
    Page = 4;
    }
}
```
Figure 7. USART1 interrupt exception and Page 3: USART Page

	Inside the game page, TIM4 is initialized to record the elapsed time of the game (that’s why it is initialized after we enter Game Page). If buffer is smaller and equal to 51 (ASCII Code of 3), the ball will bounce to the right, otherwise, the ball will bounce to the left, which is controlled by “xdir”. “xdir” refers to X-axis direction, “xdir = 1” ball moves to the right and x increases; “xdir = -1” ball moves to the left and x decreases. The angle of bounce is controlled by the increment of y. Basically, the larger the buffer number is, the bigger the y step is. In total, there are 8 different initial directions as shown in the Figure8.

Figure 8. 8 different initial directions for the ball

	The refreshment of the ball is put inside a while loop controlled by SYSTICK (task1HeartBeat count to 2 (20ms), Game Page will be refreshed). The idea is quite simple, the original ball is drawn white first and then x, y location of the ball will be updated according to the initial direction and difficulty level. Then, draw the ball red, we will see the ball moving. By the way, the difficulty level is controlled in a very clever way: the x and y step will be timed by “hard”, which is introduced before (“hard = 1” implies easy mode, “hard = 2” implies hard mode.) Thus, in hard mode, the ball will move 2 times faster than in easy mode.
```
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
                EIE3810_TFTLCD_DrawCircle(x,y, 10, 1, RED);
                EIE3810_TFTLCD_FillRectangle(x2, 80, 795, 5, BLACK);
                task1HeartBeat = 0;
            }
        }
    }
}
```
Figure 9. Page 4: Game Page
