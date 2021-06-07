/*
 * Copyright (c) 2017-2020 Arm Limited and affiliates.
 * SPDX-License-Identifier: Apache-2.0
 */

#include "mbed.h"
#include <cstdint>
#include <string>
#include "Kernel.h"
#include "HBridgeDCMotor.h"

HBridgeDCMotor m1(D9, D8);
HBridgeDCMotor m2(D6, D7);

Serial pc(SERIAL_TX, SERIAL_RX);

DigitalOut    led(LED1);
InterruptIn   *phasein1;
InterruptIn   *button;
InterruptIn   *freq;



Timer P1;
Timer P2;

EventQueue phase;

EventQueue freqmeasure;




volatile int_fast32_t x;
volatile int_fast32_t trise, tfall,i=0;
volatile float theta,theta2,theta3, theta23;
volatile float setpoint = 120.0;
volatile float phaseangle;
volatile float phase2;
volatile float phase3;

volatile bool p = false;
volatile bool running = false;
volatile bool startup = false;
volatile bool freqcheck = false;
volatile bool update = false;

volatile int_fast32_t period = 0;
volatile float per = 0;




void phase12Rise(){
    P1.start();

} 

void phase12Fall(){
    x = P1.read_us();
    P1.stop();
    P1.reset();
    phasein1->disable_irq();
    p = true;
}

void buttonSwitch(){
    
    running = !running;
    
}

void freqMeasureRise(){
    P2.start();
 }
 
 void freqMeasureFall(){
    
    period = P2.read_us(); 
    P2.stop(); 
    P2.reset();
    freq->disable_irq();
    freqcheck = true;
        
}


void measurePhaseTask(){ //+120
    led = 1;
    wait_us(1);
    InterruptIn   phasein1(PB_15);
    phasein1.rise(&phase12Rise);
    phasein1.fall(&phase12Fall);
    i =0;
    
    while (i<1) {
        if((x<50) && p){
            p = false;
            phasein1.enable_irq();
        }
        else if(p && (x > 50)){
             theta = 360.0*x/(per*1000.0);

             p = false;
             i = i +1;   
        }
        
    }
    
    led =0; 
    phasein1.fall(NULL);
    phasein1.rise(NULL);
    phasein1.enable_irq();
    update = true;
    phase.break_dispatch();
    
    
}

void measurePhaseTask12(){ //+120
    led = 1;
    wait_us(1);
    InterruptIn   phasein1(PB_15);
    phasein1.rise(&phase12Rise);
    phasein1.fall(&phase12Fall);
    i =0;
    
    while (i<1) {
        if((x<50) && p){
            p = false;
            phasein1.enable_irq();
        }
        else if(p && (x > 50)){
             theta2 = 360.0*x/(per*1000.0);
             
             p = false;
             i = i +1;   
        }
        
    }
    
    led =0; 
    phasein1.fall(NULL);
    phasein1.rise(NULL);
    phasein1.enable_irq();
    update = true;
    phase.break_dispatch();
    
    
}

void measurePhaseTask13(){ //-120
    led = 1;
    wait_us(1);
    InterruptIn   phasein1(PB_14);
    phasein1.rise(&phase12Rise);
    phasein1.fall(&phase12Fall);
    i =0;
    
    while (i<1) {
        if((x<50) && p){
            p = false;
            phasein1.enable_irq();
        }
        else if(p && (x > 50)){
             theta3 = 360.0*x/(per*1000.0);

             p = false;
             i = i +1;   
        }
        
    }
    
    led =0; 
    phasein1.fall(NULL);
    phasein1.rise(NULL);
    phasein1.enable_irq();
    update = true;
    phase.break_dispatch();
    
    
}  

void measurePhaseTask23(){ //120&240
    led = 1;
    wait_us(1);
    InterruptIn   phasein1(PB_13);
    phasein1.rise(&phase12Rise);
    phasein1.fall(&phase12Fall);
    i =0;
    
    while (i<1) {
        if((x<50) && p){
            p = false;
            phasein1.enable_irq();
        }
        else if(p && (x > 50)){
             theta23 = 360.0*x/(per*1000.0);
             p = false;
             i = i +1;   
        }
        
    }
    
    led =0; 
    phasein1.fall(NULL);
    phasein1.rise(NULL);
    phasein1.enable_irq();
    update = true;
    phase.break_dispatch();
    
    
} 

void freqMeasureTask(){
    led = 1;
    wait_us(1);
    i =0;
    InterruptIn freq(PB_1);
    freq.rise(&freqMeasureRise);
    freq.fall(&freqMeasureFall);
    
    while(i<1){
        
        if(period < 50 && freqcheck){
            freqcheck = false;
            freq.enable_irq();
        }
        
        else if(freqcheck && period >50){
        
            per = 2*period/1000.0;
            freqcheck = false;  
            i++;
        }  
    }
    
    
    led = 0;
    freq.rise(NULL);
    freq.fall(NULL);
    freq.enable_irq();
    //   pc.printf("Phase1 angle is %f ms \n", 2*x/1000.0);
    //    pc.printf("Phase1 angle is %f degrees \n", theta);
    freqmeasure.break_dispatch();    
    
    
}


int main()
{
    int w=0;
    pc.baud(115200);
    float sampleTime = 50e-3, switchingFrequency = 25e3, rampTime = 1;
    m1.configure(sampleTime, switchingFrequency, rampTime, rampTime);
    m2.configure(sampleTime, switchingFrequency, rampTime, rampTime);

    
    
    

    while(1){
        
        while(w<2){ // wait 3 seconds after powerup for start up transient to die down
        wait(1);
         pc.printf("waiting \r \n");
        w++;
        }
        
        led=!led; // blink onboard led 
        
        
        freqmeasure.call(freqMeasureTask);
        freqmeasure.dispatch();
        wait_us(100);
        
        
        phase.call(measurePhaseTask13);
        phase.dispatch();
        wait_us(100);
        
        phase.call(measurePhaseTask12);
        phase.dispatch();
        wait_us(100);
        
        

        
                
        if(update){
            
            pc.printf("Period  in ms is %.4f \r \n", per);
            pc.printf("Frequency in Hz is %.4f \r \n", (1/(per/1000)));
            pc.printf("Phase 1-2 angle is %.4f degrees \r \n", theta2);
            pc.printf("Phase 1-3 angle is %.4f degrees \r \n", theta3);
            
            
                                
            if(theta3 < (0.998*setpoint)){
                m1.setDutyCycle(1); //motor right

            }
            else if(theta3 > 1.002*setpoint){
                m1.setDutyCycle(-1); //motor left
              
            }
            
            else{
                m1.coast();
               
            }

            
            if(theta2 < (0.998*setpoint)){
                m2.setDutyCycle(-1); //motor left

            }
            else if(theta2 > 1.002*setpoint){
                m2.setDutyCycle(1); //motor right
              
            }
            
            else{
                m2.coast();
            } 
            
            
            wait_us(100);
            update = false;
            ThisThread::sleep_for(100); 
        
        }
        
        
    }
    
}