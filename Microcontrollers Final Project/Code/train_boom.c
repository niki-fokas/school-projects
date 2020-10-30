// Timing code to control the servo

void middle(){
  digitalWrite(0,HIGH);
  delayMicroseconds(1500);
  digitalWrite(0,LOW);
  delayMicroseconds(18500);
}

void right(){
  digitalWrite(0,HIGH);
  delayMicroseconds(2000);
  digitalWrite(0,LOW);
  delayMicroseconds(18000);
}

void left(){
  digitalWrite(0,HIGH);
  delayMicroseconds(1000);
  digitalWrite(0,LOW);
  delayMicroseconds(19000);
}

void boomDown(){
      int n=0;
      int y=0;
     while(y<6){
        n = 0;
        while(n<1){
          left();
          n++;
          
        }
        delay(500);
        y++;
     }
}

void boomUp(){
      int n=0;
      int y=0;
     while(y<10){
        n = 0;
        while(n<1){
          right();
          n++;
          
        }
        delay(500);
        y++;
     }
}  

void boomHomePos(){
    int z=0;
    int n =0;
    while(z<1){       
          while(n<50){
            right();
            n++;
          }
          z++;
      }
}

void setup() {


pinMode(2,INPUT);
pinMode(4,INPUT);
pinMode(0,OUTPUT);





}

//main code
unsigned int n=0;
unsigned int y=0;
unsigned int x =0;
int z = 0;
void loop() {
	
	//on powerup return the boom to the home position
   while(z<1){
       boomHomePos();
        z++;
    }
	y = digitalRead(4); 
	x = digitalRead(2); //incoming train
   
	n = x+(2*y)
  
	switch(n){
	   case 1:
			boomDown();
			break;
		
		case 2:
			boomUp();
			break;
			
		default: 
			digitalWrite(0,LOW);
			break;
			
	}
    
  }
   


            
 
           
               
    
