//tested to 65000hz//

//variables to keep track of the timing of recent interrupts
long x = 0;

long curr_time=0;
int rpm;
void setup() {               
  //enable interrupt 0 which uses pin  2
  //jump to the increment function on falling edge
  attachInterrupt(0, increment, FALLING);
  Serial.begin(115200);  //turn on serial communication
 
  curr_time = micros();
}

void loop() {
      rpm = x*30;
       x=0;
   Serial.print("RPM is ");
   Serial.println(rpm);
    delay(1000);  // basically count the number of falling edge (interrupts) per second!

}

// Interrupt service routine for interrupt 0
void increment() {
 
  x++;
}
