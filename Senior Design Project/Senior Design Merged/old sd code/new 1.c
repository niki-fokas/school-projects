#include "mbed.h"
#include "HBridgeDCMotor.h"

HBridgeDCMotor m1(D9, D8);
HBridgeDCMotor m2(D6, D7);
AnalogIn pot(A4);
Serial pc(SERIAL_TX, SERIAL_RX);

float v =0;
double phaseangle;
double setpoint =120.0;
int main() {
    pc.baud(115200);
    float sampleTime = 50e-4, switchingFrequency = 25e3, rampTime = 1;
    m1.configure(sampleTime, switchingFrequency, rampTime, rampTime);
    m2.configure(sampleTime, switchingFrequency, rampTime, rampTime);
    //m1.setDutyCycle(1);
    
    while(true) {
       /*m1.setDutyCycle(1); // left
        //m2.setDutyCycle(1);
        wait(1);
        v = pot.read();
        pc.printf(" Pot val is %.5f \r \n",v);
        m1.setDutyCycle(-.5); //right
        wait(1);
        v = pot.read();
        pc.printf(" Pot val is %.5f \r \n",v);*/
        v = pot.read();
        phaseangle = floor(100.0*v*360.0)/100.0;

        
        pc.printf(" Pot val is %.5f \r \n",phaseangle);
        
        if(phaseangle < (0.99*setpoint)){
            m1.setDutyCycle(-1); //motor right.... increase pot value
            //pc.printf(" Pot val is %.5f \r \n",phaseangle);
             v = pot.read();
            phaseangle = floor(100.0*v*360.0)/100.0;
            wait_ms(25);
        }
        else if(phaseangle > 1.01*setpoint){
            m1.setDutyCycle(1); //motor left ... decrease potvalue
            pc.printf(" Pot val is %.5f \r \n",phaseangle);
            //phaseangle = floor(100.0*pot.read()*360.0)/100;
             v = pot.read();
            phaseangle = floor(100.0*v*360.0)/100.0;
            wait_ms(25);
        }
        
        else{ 
            phaseangle = floor(100.0*pot.read()*360.0)/100;
            m1.coast();
            pc.printf(" Pot val is %.5f \r \n",phaseangle);
        }
        
      }  
        
    
}