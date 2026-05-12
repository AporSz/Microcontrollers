// Adafruit Motor shield library
// copyright Adafruit Industries LLC, 2009
// this code is public domain, enjoy!

#include <AFMotor.h>

// DC motor on M4 M3 ...
AF_DCMotor motor(3);

int potmeter;
int speed;

void setup() 
{
  Serial.begin(9600);           
  Serial.println("DC Motor controlled with PWM");
 
  motor.setSpeed(80);
  motor.run(RELEASE);
}



void loop() 
{
  motor.run(FORWARD);

   potmeter = analogRead(A0);
   speed = potmeter / 8;
   Serial.print("Potentiometer = "); Serial.println(potmeter);
   Serial.print("Speed = "); Serial.println(speed);
   Serial.println();
   
   motor.setSpeed(speed);  
   delay(3);

}
