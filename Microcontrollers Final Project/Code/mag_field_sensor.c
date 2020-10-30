//adc for magnetic field sensors

unsigned int S1;
unsigned int S2;
unsigned int S3;
void setup() {

  //pin setup
pinMode(A0,INPUT);
pinMode(A1,INPUT);
pinMode(A2,INPUT);
pinMode(2,OUTPUT);
pinMode(3,OUTPUT);
pinMode(4,OUTPUT);
Serial.begin(9600);

}

void loop() {
     
     S1 = analogRead(A0);
	 S2 = analogRead(A1);
     S3 = analogRead(A2);
     Serial.print("S1 "); Serial.print(S1); Serial.print(" "); Serial.print("S2 ");Serial.print(S2); Serial.print(" ");  Serial.print("S3 ");Serial.println(S3);
  
     if(S1 < 490){
      digitalWrite(2,HIGH);
     }
     if(S1 > 490){
      digitalWrite(2,LOW);
     }
	
	 if(S2 < 490){
      digitalWrite(3,HIGH);
     }
     if(S2 > 490){
      digitalWrite(3,LOW);
     }

     if(S3 < 490){
      digitalWrite(4,HIGH);
     }
     if(S3 > 490){
      digitalWrite(4,LOW);
     }
     
}	

            
 
           
               
    
