//this code is for attiny85 controling the train lights
void light_flash(){
    digitalWrite(0,HIGH);
    digitalWrite(1,LOW);
    delay(250);
    
    digitalWrite(0,LOW);
    digitalWrite(1,HIGH);
    delay(250);
}

void setup() {

  // put your setup code here, to run once:
pinMode(2,INPUT);
pinMode(4,INPUT);
pinMode(0,OUTPUT);
pinMode(1,OUTPUT);





}
int n=0;
unsigned int y=0;
unsigned int x =0;
int z = 0;
void loop() {
    
    
   y = digitalRead(4);
   x = digitalRead(2);
   if(x == 1){
      while(y == 0){
         light_flash();
         y = digitalRead(4);
      } 
   
   }
   
   if(y == 1){
      n=0;
      while(n<10){
         light_flash();
         n++;
      }
      digitalWrite(0,LOW);
      digitalWrite(1,LOW);
    }
    
    else{
      digitalWrite(0,LOW);
      digitalWrite(1,LOW);
    }
     
 }
   


            
 
           
               
    
