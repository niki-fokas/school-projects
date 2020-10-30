// This code controls the lcd and traffic lights based on train and sensor input

#include "TM4C123GH6PM.h"

#define LCD_PORT GPIOB 
#define RS 1    /* BIT0 mask */ 
#define RW 2    /* BIT1 mask */ 
#define EN 4    /* BIT2 mask */

void delay_Ms(uint16_t x){
   SysTick->LOAD=(16000*x)-1;    //RELOAD VALUE goes here
   SysTick->CTRL|=0x5;        //Triggering with external clock
   while((SysTick->CTRL&(1<<16))==0);//monitoring bit 16 to be set 
	SysTick->CTRL=0;      //Disabling SysTick Timer
}

void delay_Us(uint16_t x){
   SysTick->LOAD=(16*x)-1;    //RELOAD VALUE goes here
   SysTick->CTRL|=0x5;        //Triggering with external clock
   while((SysTick->CTRL&(1<<16))==0);//monitoring bit 16 to be set 
	SysTick->CTRL=0;      //Disabling SysTick Timer
}

void delayMs(int n);
void delayUs(int n);
void LCD_nibble_write(unsigned char data, unsigned char control);
void LCD_command(unsigned char command);
void LCD_data(unsigned char data);
void LCD_init(void);

void LCD_Left(){
	
				LCD_command(1);       /* clear display */
        LCD_command(0x80);    /* LCD cursor location */
				delayMs(500);
        LCD_data('<');
        LCD_data('-');
        LCD_data('-');
        LCD_data('-');
        LCD_data('-');
				LCD_data('-');
        LCD_data('-');
        LCD_data('-');
        LCD_data('-');
				LCD_data('-');
        LCD_data('-');
        LCD_data('-');
        LCD_data('-');
				LCD_data('-');
        LCD_data('-');
        LCD_data('-');
        delayMs(500);

}

void LCD_Right(){
				LCD_command(1);       /* clear display */
        LCD_command(0x80);    /* LCD cursor location */
				delayMs(500);
        LCD_data('-');
        LCD_data('-');
        LCD_data('-');
        LCD_data('-');
        LCD_data('-');
				LCD_data('-');
        LCD_data('-');
        LCD_data('-');
        LCD_data('-');
				LCD_data('-');
        LCD_data('-');
        LCD_data('-');
        LCD_data('-');
				LCD_data('-');
        LCD_data('-');
        LCD_data('>');
        delayMs(500);
	
}

void GPIOE_Handler(void)
{
    int i;
    volatile int readback;
    
	while(GPIOE->MIS != 0){
	
			if(GPIOE->MIS & 0x10){
				for (i = 0; i < 1; i++)
				{
					//yes train
					while(GPIOE->DATA != 0x20){
						LCD_Right();
					}
				}
				
				GPIOE->ICR |= 0x10; 
				readback = GPIOE->ICR;
			}
			
			
		else{
			GPIOE->ICR = GPIOE->MIS; 
			readback = GPIOE->ICR; 
			
		}
	}
}



void LCD_init(void)
{
    SYSCTL->RCGCGPIO |= 0x02;   /* enable clock to GPIOB */ 
    LCD_PORT->DIR = 0xFF;       /* set all PORTB pins as output */
    LCD_PORT->DEN = 0xFF;       /* set all PORTB pins as digital pins */

    delayMs(20);                /* initialization sequence */
    LCD_nibble_write(0x30, 0);
    delayMs(5);
    LCD_nibble_write(0x30, 0);
    delayUs(100);
    LCD_nibble_write(0x30, 0);
    delayUs(40);

    LCD_nibble_write(0x20, 0);  /* use 4-bit data mode */
    delayUs(40);
    LCD_command(0x28);          /* set 4-bit data, 2-line, 5x7 font */
    LCD_command(0x06);          /* move cursor right */
    LCD_command(0x01);          /* clear screen, move cursor to home */
    LCD_command(0x0F);          /* turn on display, cursor blinking */
}

void LCD_nibble_write(unsigned char data, unsigned char control)
{
    data &= 0xF0;       /* clear lower nibble for control */
    control &= 0x0F;    /* clear upper nibble for data */
    LCD_PORT->DATA = data | control;       /* RS = 0, R/W = 0 */
    LCD_PORT->DATA = data | control | EN;  /* pulse E */
    delayUs(0);
    LCD_PORT->DATA = data;
    LCD_PORT->DATA = 0;
}

void LCD_command(unsigned char command)
{
    LCD_nibble_write(command & 0xF0, 0);   /* upper nibble first */
    LCD_nibble_write(command << 4, 0);     /* then lower nibble */
    
    if (command < 4)
        delayMs(2);         /* commands 1 and 2 need up to 1.64ms */
    else
        delayUs(40);        /* all others 40 us */
}

void LCD_data(unsigned char data)
{
    LCD_nibble_write(data & 0xF0, RS);    /* upper nibble first */
    LCD_nibble_write(data << 4, RS);      /* then lower nibble  */
    
    delayUs(40);
}


/* delay n milliseconds (16 MHz CPU clock) */
void delayMs(int n)
{
    int i, j;
    for(i = 0 ; i < n; i++)
        for(j = 0; j < 3180; j++)
            {}  /* do nothing for 1 ms */
}

/* delay n microseconds (16 MHz CPU clock) */
void delayUs(int n)
{
    int i, j;
    for(i = 0 ; i < n; i++)
        for(j = 0; j < 3; j++)
            {}  /* do nothing for 1 us */
}

/* This function is called by the startup assembly code to perform system specific initialization tasks. */
void SystemInit(void)
{
    /* Grant coprocessor access */
    /* This is required since TM4C123G has a floating point coprocessor */
    SCB->CPACR |= 0x00f00000;
}


int main(void)
{
		unsigned int BACKUP =0;
		unsigned int magsensor1 =0;
		unsigned int magsensor2 =0;
		int x =0;
		SYSCTL->RCGCGPIO |= 0x01;	//GPIOA clck
		SYSCTL->RCGCGPIO |= 0x10; //GPIOE clck
	
		GPIOA->DIR = 0xFc;					//port a outputs
		GPIOA->DEN = 0xFc;					// port a inputs and outputs
	
		GPIOE->DIR = 0x00; 					//port E outputs
		GPIOE->DEN = 0x3F; 					//port E inputs and outputs
    LCD_init();
		
		/* configure PORTE, for rising edge trigger interrupt */
			GPIOE->IS  &= ~0x10;        /* make bit 4 edge sensitive */
			GPIOE->IBE &= 0x10;        /* trigger is controlled by IEV */
			GPIOE->IEV &= 0x10;        /* rising edge trigger */
			GPIOE->ICR |= 0x10;         /* clear any prior interrupt */
			GPIOE->IM  |= 0x10;         /* unmask interrupt */
		 /* enable interrupt in NVIC and set priority to 3 */
			NVIC->IP[4] = 3 << 5;     /* set portE interrupt priority to 5 */
			NVIC->ISER[0] |= 0x00000010;  /* enable IRQ4 (bit 5 of ISER[0]) */
		
			__enable_irq(); /* global enable IRQs */
    while(1)
    {
				/*
							SORTING ALGORITHM
							
					RAIL SENSORS = RS
					INTERSECTION ONE (with train) NS = NS1
					INTERSECTION TWO (bypass) NS = NS2
					INTERSECTION ONE (with train) EW = EW1
					INTERSECTION TWO (bypass) EW = EW2
					BACKUP SENSOR = BACKUP
					LCD DISPLAY SIGNAGE = LCD
					*/
					LCD_Left();
					BACKUP = GPIOE->DATA & 0x01; // bit 0
					magsensor1 = GPIOE->DATA & 0x02; // bit 1
					magsensor2 = GPIOE->DATA & 0x04; //bit 3
					x=0;
					while (magsensor2 == 0x04){
								GPIOA->DATA=0;
								GPIOA->DATA=0x08; // NS2G EW2R
								GPIOA->DATA=0;
								magsensor2 = GPIOE->DATA & 0x04;
							}		
			
			
						while(magsensor1 == 0x02){
								BACKUP = GPIOE->DATA & 0x01; // bit 0
								magsensor1 = GPIOE->DATA & 0x02; // bit 1
								magsensor2 = GPIOE->DATA & 0x04; //bit 3
									if (x>5){
										BACKUP = GPIOE->DATA & 0x01;
										magsensor1 = GPIOE->DATA & 0x02;
										GPIOA->DATA = 0; //A2
										GPIOA->DATA = 0x04; //A2
										GPIOA->DATA=0;
								 while(BACKUP == 0x01){		
										GPIOA->DATA = 0; //A2
										GPIOA->DATA = 0x04; //A2
										GPIOA->DATA=0;
									  LCD_Right();
										delayMs(100);
									  x=0;
									 BACKUP = GPIOE->DATA & 0x01;
								 }									 
								} 
								else {
									LCD_Left();
									x++;
								}
							}
							
						
							
							
				
						//while there is no train, do the following
						
    }

	}
