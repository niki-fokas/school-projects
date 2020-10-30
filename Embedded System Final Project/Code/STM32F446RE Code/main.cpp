#include "mbed.h"
#include "components.h"
#include "hcsr04.h"
#include "TextLCD.h"
#include "math.h"

Timer sysTimer;
Serial pc(SERIAL_TX, SERIAL_RX);
HCSR04 sensor(D8, D9);
PwmOut motor(D6);
PwmOut motor2(D5);
TextLCD lcd(PC_4, PB_13, PB_14, PB_15, PB_1, PB_2); // RS, E, D4-D7


void printInt(int z){
        pc.printf(" %d \n\r",z); 
    }
    
void printFloat(float z){
        pc.printf(" %.1f \n\r",z);
    }
 
int height = 10;
float mos = 0.0f;
float mos2 = 1.0f - mos;
float x = 0;
int read = 0;
unsigned int avg = 0;
int y = 0;
int i =0;
int t1,t2;
char c;
int l,m,n;

  

int heliCalibrate(){
    
    while(i<20){
        
        read = abs(y - sensor.distance());
        if(read<0 || read > 30){
            read = abs(y - sensor.distance());
        }
        x = x + read;
        i++;
        
    }      
    unsigned   int d = floor(x/20);
    printInt(d);
    return d;
    
}


int avgHeight(){
    
    i =0;
    while(i<10){
        read = abs(y - sensor.distance());
        if(read<0 || read > 30){
            read = abs(y - sensor.distance());
        }
        avg = avg + read;
        i++;
        wait_ms(1);
    }
    return avg/10;
}

int main()
{
    pc.baud(115200);
    motor.period_ms(1);
    motor.write(0.1f);
    motor2.period_ms(1);
    motor2.write(0.1f);
    y = heliCalibrate();
    lcd.cls();
 while(1){   
        if(pc.readable()){ //read keyboard input from dummy terminal
                c = pc.getc();
                l = c - '0';
                c = pc.getc();
                m = c - '0';
                n = l+m;
                pc.printf(" %d \n\r",n);
              
                height = n;                     
            }
        avg = avgHeight();
        while(avg<height && mos<=1.0f && (t1<=60.0f && t2<=60.0f)){
            avg = avgHeight();
            mos = mos+0.05f;
            mos2 = 1.0f - mos;
            motor.write(mos);
            motor2.write(mos2);
           
            t1 = mosTemp(); t2 = mosTemp2();
          }
        while(avg>height && mos>= .75f && (t1<=60.0f && t2<=60.0f)){
            avg = avgHeight();
            mos= mos-0.05f;
            mos2 = 1.0f - mos;
            motor.write(mos);
            motor2.write(mos2);
         
            t1 = mosTemp(); t2 = mosTemp2();
            
        }
        
        if((t1 || t2) > 60.0f){ //if mosfets get to hot
            i = 0;
            while(i<20){ //ramp motors down over a 200ms window
                mos = mos-0.05f;
                mos2 = 1.0f - mos;
                motor.write(mos);
                motor2.write(mos2);
                wait_ms(10);
                i++; 
            }
            wait(60); //wait 1 minute to cool off
            t1 = mosTemp(); t2 = mosTemp2(); //check temp again
        } 
                
        pc.printf("Distance Is \n\r");
        printInt(avg);
        pc.printf("Duty Cycle is \n\r");
        printFloat(mos);
        pc.printf("Temperature 1 is \n\r");
        printFloat(mosTemp());
        pc.printf("Temperature 2 is \n\r");
        printFloat(mosTemp2());
      
         t1 = mosTemp(); t2 = mosTemp2();
        lcd.locate(0,0);
        lcd.printf("H:%dcm HMax:%dcm",avg,height);
        lcd.locate(0,1);
        lcd.printf("T1:%d C,T2:%d C",t1,t2);
              
   } 

 }
