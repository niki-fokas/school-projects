#include "mbed.h"
#include "TextLCD.h"
#include <cmath>


AnalogIn Mos1(A1);
AnalogIn Mos2(A3);
 
float mosTemp(){
    float tempC,tempF,a[10],avg = 0.0f;
    int i =0;
 
    
 
        avg=0;
        for(i=0;i<10;i++) {
            a[i]=Mos1.read();
            //wait_ms(1);
        }
        
        for(i=0;i<10;i++) {
            
            avg=avg+(a[i]/10);
        }
 
        
        tempF=(9.0f*tempC)/5.0f + 32.0f;
        return tempC=((avg*3.3f-.055f)*100.0f);
}

float mosTemp2(){
    float tempC,tempF,a[10],avg = 0.0f;
    int i =0;
 
    
 
        avg=0;
        for(i=0;i<10;i++) {
            a[i]=Mos2.read();
            //wait(.02);
        }
        
        for(i=0;i<10;i++) {
            
            avg=avg+(a[i]/10);
        }
 
        
        tempF=(9.0f*tempC)/5.0f + 32.0f;
        return tempC =((avg*3.3f-.055f)*100.0f);

}




        