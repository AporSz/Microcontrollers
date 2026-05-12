// Adafruit Motor shield library
// copyright Adafruit Industries LLC, 2009
// this code is public domain, enjoy!

#include <AFMotor.h>

// DC motor on M4 M3
AF_DCMotor motor(3);

int i;

void setup() 
{
  Serial.begin(9600);           
  Serial.println("DC Motor with PWM");
 
  motor.setSpeed(200);
  motor.run(RELEASE);
}



void loop() 
{
  motor.run(FORWARD);
  
  for (i=0; i<255; i++) 
  {
    motor.setSpeed(i);  
    delay(3);
  }
 
  for (i=255; i>0; i--) 
  {
    motor.setSpeed(i);  
    delay(3);
  }
 
  motor.run(BACKWARD);
  
  for (i=0; i<255; i++) 
  {
    motor.setSpeed(i);  
    delay(3);
  }
 
  for (i=255; i>0; i--) 
  {
    motor.setSpeed(i);  
    delay(3);
  }

}
