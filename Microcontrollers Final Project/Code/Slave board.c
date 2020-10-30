#include "TM4C123GH6PM.h"


#include <math.h>

/*delay n milliseconds (16 MHz CPU clock) */
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
// delay software
void delayMs(int n)
{
    int i, j;
    for(i = 0 ; i < n; i++)
        for(j = 0; j < 3180; j++)
            {}  // do nothing for 1 ms
}

void sevenseg(){
						int i =0;
						GPIOB->DATA = 0x6F; //9
						delay_Ms(500);
						
						GPIOB->DATA = 0x7F; //8
						delay_Ms(500);
						
						GPIOB->DATA = 0x07; //7
						delay_Ms(500);
						
						GPIOB->DATA = 0x7D; //6
						delay_Ms(500);
						
						GPIOB->DATA = 0x6D; //5
						delay_Ms(500);
						
						GPIOB->DATA = 0x66; //4
						delay_Ms(500);
						
						GPIOB->DATA = 0x4F; //3
						delay_Ms(500);
						
						GPIOB->DATA = 0x5B; //2
						delay_Ms(500);
						
						GPIOB->DATA = 0x06; //1
						delay_Ms(500);
						
						for (i = 0; i < 4; i++){
							GPIOB->DATA = 0x3F; //0
							delay_Ms(500);
						}
	
}

void GPIOC_Handler(void)
{
    int i;
    volatile int readback;
		while(GPIOC->MIS != 0){
			
			if(GPIOC->MIS & 0x10){
				for (i = 0; i < 1; i++)
				{
					if (GPIOA->DATA != 0x84){
						GPIOA->DATA = 0x30;				//NS R EW G 
						delay_Ms(1000);
					
						GPIOA->DATA = 0x50;     // NS R EW Y
						delay_Ms(2000);
						
						GPIOA->DATA = 0x90; // NS R EW R
						delay_Ms(1000);
									
						GPIOA->DATA = 0x84;     //NS G EW R
						delay_Ms(5000);
					}
					else{
					GPIOA->DATA=0x84;
					}
					
					
				}
				GPIOC->ICR |= 0x10; //clear the interrupt flag before return 
				readback = GPIOC->ICR;
			}
			
			if(GPIOC->MIS & 0x20){
				for (i = 0; i < 1; i++)
				{
					if (GPIOD->DATA!=0x0C){
						GPIOD->DATA = 0x81; //NS R EW Y
						GPIOD->DATA = 0x82;
						delay_Ms(2000);							
				
						GPIOD->DATA = 0x84; // NS R EW R
						delay_Ms(1000);
																			
						GPIOD->DATA = 0x0C; //NS G EW R 
						delay_Ms(5000);
					}
					else{
						GPIOD->DATA=0x0C;
					}
					
				}
				GPIOC->ICR |= 0x20; 
				readback = GPIOC->ICR;
			}
			
			if(GPIOC->MIS & 0x40 && GPIOD->DATA==0x81){
				for (i = 0; i != 1; i++)
				{
						 
							GPIOD->DATA = 0x82; // NS R EW Y
							delay_Ms(2000);
							GPIOD->DATA=0;
							GPIOD->DATA = 0x84; // NS R EW R
							delay_Ms(2000);
							//sevenseg();
							delay_Ms(5000);
							GPIOD->DATA=0;
							GPIOD->DATA=0x0C; //NSG EWR
					
				}
				GPIOC->ICR |= 0x40; 
				readback = GPIOC->ICR;
			}
			
			if(GPIOC->MIS & 0x40 && GPIOD->DATA==0x0C){
				for (i = 0; i != 1; i++)
				{
						//if NSG EWR
							GPIOD->DATA = 0x44; // NS Y EW R
							delay_Ms(2000);
							GPIOD->DATA=0;
							GPIOD->DATA = 0x84; // NS R EW R
							delay_Ms(2000);
							//sevenseg();
							delay_Ms(5000); 
							GPIOD->DATA=0;
							GPIOD->DATA=0x81; //NSR EWG
				}
				GPIOC->ICR |= 0x40; 
				readback = GPIOC->ICR;
			}
			
			
				 
						
			
			if(GPIOC->MIS & 0x80 && GPIOA->DATA==0x30){
				for (i = 0; i != 1; i++)
				{
					 // if NSR EWG
						GPIOA->DATA = 0x50; // NS R EW Y
						delay_Ms(2000);
						GPIOA->DATA=0;
						GPIOA->DATA = 0x90; // NS R EW R
						delay_Ms(2000);
						//sevenseg();
						delay_Ms(5000);
						GPIOA->DATA=0;
						GPIOA->DATA=0x84;
		
				}
				GPIOC->ICR |= 0x80; 
				readback = GPIOC->ICR;
			}
			
			if(GPIOC->MIS & 0x80 && GPIOA->DATA==0x84){
				for (i = 0; i != 1; i++)
				{
					  //if NSG EWR
						GPIOA->DATA = 0x88; // NS Y EW R
						delay_Ms(2000);
						GPIOA->DATA=0;
						GPIOA->DATA = 0x90; // NS R EW R
						delay_Ms(2000);
						//sevenseg();
						delay_Ms(5000);
						GPIOA->DATA=0;
						GPIOA->DATA=0x30;
					
		
				}
				GPIOC->ICR |= 0x80; 
				readback = GPIOC->ICR;
			}
			
				
				else{
				GPIOC->ICR = GPIOC->MIS; 
				readback = GPIOC->ICR;     // a read to force clearing of interrupt flag 
				}
		}
}


void GPIOE_Handler(void)
{
    int i;
    volatile int readback;
    
	while(GPIOE->MIS != 0){
			if(GPIOE->MIS & 0x01){
				for (i = 0; i < 1; i++)
				{
					
					if (GPIOA->DATA != 0x30){
						GPIOA->DATA = 0x84; //NS Y EW R
						GPIOA->DATA = 0x88;
						delay_Ms(2000);									
						GPIOA->DATA = 0x90; // NS R EW R
						delay_Ms(1000);								       
						GPIOA->DATA = 0x30; //NS R EW G 
						delay_Ms(5000);	
					
					}
					else{
						GPIOA->DATA=0x30;
					}
			
				}
				
				GPIOE->ICR |= 0x01; 
				readback = GPIOE->ICR;
			}
		if(GPIOE->MIS & 0x02){
				for (i = 0; i < 1; i++)
				{
					if (GPIOA->DATA != 0x30){
						GPIOA->DATA = 0x84; 	//NS Y EW R
						GPIOA->DATA = 0x88;
						delay_Ms(2000);							
				
						GPIOA->DATA = 0x90; // NS R EW R
						delay_Ms(1000);
																		 
						GPIOA->DATA = 0x30; //NS R EW G 
						delay_Ms(5000);
					}
					else{
						GPIOA->DATA=0x30;
					}
		
				}
				
				GPIOE->ICR |= 0x02; 
				readback = GPIOE->ICR;
			}
	
		if(GPIOE->MIS & 0x04){
				for (i = 0; i < 1; i++)
				{					
					if (GPIOA->DATA !=0x84){
						GPIOA->DATA = 0x30;				//NS R EW G 
						GPIOA->DATA = 0x50;     // NS R EW Y
						delay_Ms(2000);
					
						GPIOA->DATA = 0x90; // NS R EW R
						delay_Ms(1000);
									
						GPIOA->DATA = 0x84;     //NS G EW R	
						delay_Ms(5000);
					}
					else{
						GPIOA->DATA=0x84;
					}
					
				}
				GPIOE->ICR |= 0x04; 
				readback = GPIOE->ICR;
			}
		
		if(GPIOE->MIS & 0x08){
				for (i = 0; i < 1; i++)
				{
					if (GPIOD->DATA != 0x81){
						GPIOD->DATA = 0x0C; //NS Y EW R
						GPIOD->DATA = 0x44;
						delay_Ms(2000);							
				
						GPIOD->DATA = 0x84; // NS R EW R
						delay_Ms(1000);
															    
						GPIOD->DATA = 0x81; //NS R EW G    
						delay_Ms(5000);
					}
					else{
						GPIOD->DATA=0x81;
					}
					
				}
				
				GPIOE->ICR |= 0x08; 
				readback = GPIOE->ICR;
			}
			
		if(GPIOE->MIS & 0x10){
				for (i = 0; i < 1; i++)
				{
					if (GPIOD->DATA !=0x81){
						GPIOD->DATA = 0x0C;//NS Y EW R
						GPIOD->DATA = 0x44;
						delay_Ms(2000);							
				
						GPIOD->DATA = 0x84; // NS R EW R
						delay_Ms(1000);
															//NS R EW G        
						GPIOD->DATA = 0x81; 
						delay_Ms(5000);
					}
					else{
						GPIOD->DATA=0x81;
					}
					
				}
				
				GPIOE->ICR |= 0x10; 
				readback = GPIOE->ICR;
			}
			
			if(GPIOE->MIS & 0x20){
				for (i = 0; i < 1; i++)
				{	
					if (GPIOD->DATA != 0x0C){
						GPIOD->DATA = 0x81; //NS R EW G 										
						GPIOD->DATA = 0x82; //NS R EW Y
						delay_Ms(2000);							
				
						GPIOD->DATA = 0x84; // NS R EW R
						delay_Ms(1000);
															        
						GPIOD->DATA = 0x0C; //NS G EW R
						delay_Ms(5000);		
					}
					else{
						GPIOD->DATA=0x0C;
					}
				
				}
				
				GPIOE->ICR |= 0x20; 
				readback = GPIOE->ICR;
			}
			
		else{
			GPIOE->ICR = GPIOE->MIS; 
			readback = GPIOE->ICR;     
		}
	}
}

int x =0;
int main(void)
{		
	
		SYSCTL->RCGCGPIO |= 0x01; //GPIOA clck
		SYSCTL->RCGCGPIO |= 0x02; //GPIOB clck
		SYSCTL->RCGCGPIO |= 0x04; //GPIOC clck
		SYSCTL->RCGCGPIO |= 0x08; //GPIOD clck
		SYSCTL->RCGCGPIO |= 0x10; //GPIOE clck
	
		GPIOA->DIR = 0xFC;					//port a outputs
		GPIOA->DEN = 0xFC;					// port a inputs and outputs
	
		GPIOB->DIR = 0xFF;					//port b outputs
		GPIOB->DEN = 0xFF;					// port b inputs and outputs
		
		
		GPIOC->LOCK = 0x4C4F434B;			//unlock port C
		GPIOC->CR |= 0xF0;					//reconifgure port c
		GPIOC->DIR = 0x00; 					//port C outputs
		GPIOC->DEN = 0xF0; 					//port C inputs and outputs
	
		GPIOD->LOCK = 0x4C4F434B;			//unlock port d
		GPIOD->CR |= 0xCF;					//reconifgure port d
		GPIOD->DIR = 0xCF; 					//port D outputs
		GPIOD->DEN = 0xCF; 					//port D inputs and outputs
		
		GPIOE->DIR = 0x00; 					//port E outputs
		GPIOE->DEN = 0x3F; 					//port E inputs and outputs
		
		
	
		/* configure PORTE, for rising edge trigger interrupt */
			GPIOE->IS  &= ~0x3F;        /* make bits 0,1,2,3,4,5 edge sensitive */
			GPIOE->IBE &= 0x3F;        /* trigger is controlled by IEV */
			GPIOE->IEV &= 0x3F;        /* rising edge trigger */
			GPIOE->ICR |= 0x3F;         /* clear any prior interrupt */
			GPIOE->IM  |= 0x3F;         /* unmask interrupt */
	
		/* configure PORTC, for rising edge trigger interrupt */
			GPIOC->IS  &= ~0xF0;        /* make bits 4,5,6,7 edge sensitive */
			GPIOC->IBE &= 0xF0;        /* trigger is controlled by IEV */
			GPIOC->IEV &= 0xF0;        /* rising edge trigger */
			GPIOC->ICR |= 0xF0;         /* clear any prior interrupt */
			GPIOC->IM  |= 0xF0;         /* unmask interrupt */
    
	
		/* enable interrupt in NVIC and set priority*/
			NVIC->IP[4] = 5 << 5;     /* set portE interrupt priority to 5 */
			NVIC->ISER[0] |= 0x00000010;  /* enable IRQ4 (bit 5 of ISER[0]) */
		
			NVIC->IP[2] = 3 << 5;     /* set portC interrupt priority to 3 */
			NVIC->ISER[0] |= 0x00000004;  /* enable IRQ2 (bit 3 of ISER[0]) */

			__enable_irq(); /* global enable IRQs */
    
		
		while(1){
			//run initial light setup
			while(x<1){
				GPIOA->DATA = 0x30;			
				GPIOD->DATA = 0x81;
				x++;
				GPIOB->DATA =0;
				
			}
			
		}
		
	}
