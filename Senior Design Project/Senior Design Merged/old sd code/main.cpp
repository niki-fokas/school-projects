/*
 * Copyright (c) 2017-2020 Arm Limited and affiliates.
 * SPDX-License-Identifier: Apache-2.0
 */

#include "mbed.h"
#include <cstdint>
Serial pc(SERIAL_TX, SERIAL_RX);
// Initialize a pins to perform analog input and digital output functions
//InterruptIn   phasein1(A1,PullDown);
DigitalOut    led(LED1);
InterruptIn   *phasein1;
InterruptIn   *button;



Timer P1;
Timer P2;
Timer P3;
EventQueue phase;

volatile int_fast32_t x =0;
volatile int_fast32_t trise, tfall;

volatile bool p = false;
volatile bool running = false;



void phase12Rise(){
    trise = P1.read_us();

} 

void phase12Fall(){
    tfall = P1.read_us();
    x = tfall - trise;
    phasein1->disable_irq();
    p = true;
}

void buttonSwitch(){
	
	running = !running;
	
}

void measurePhaseTask(){
    InterruptIn   phasein1(USER_BUTTON);
	phasein1.fall(&phase12Rise);
    phasein1.rise(&phase12Fall);
	int z,y =0;
	int  i =0;
	while (i<2) {
        
        if(p){
            
            z = x;
            pc.printf("Phase1 freq is %d mS \n",z/1000);
          
             P1.reset();
             p = false;
             phasein1.enable_irq();
             i = i +1;
        }
		
    } 
	phasein1.fall(NULL);
    phasein1.rise(NULL);
	phasein1.enable_irq();
    running = false;
    phase.break_dispatch();
}	
 

 
int main()
{
    button = new InterruptIn(USER_BUTTON);
    button->rise(&buttonSwitch);
    pc.baud(115200);
    P1.start(); 
    
    
	while(1){
		led = !led;
	    ThisThread::sleep_for(500);
		if(running){
            delete button;
            pc.printf("output \n");
            phase.call(measurePhaseTask);
            phase.dispatch();
            button = new InterruptIn(USER_BUTTON);
            button->rise(&buttonSwitch);
        
		}
		
		
    }
	

   
}
