// Adafruit Motor shield library
// copyright Adafruit Industries LLC, 2009
// this code is public domain, enjoy!

#include <AFMotor.h>

AF_DCMotor stanga(1);
AF_DCMotor dreapta(2);

void setup() {
  Serial.begin(9600);  // set up Serial library at 9600 bps
  Serial.println("Motor test!");

  // turn on motor
  stanga.setSpeed(200);
  stanga.run(RELEASE);

  dreapta.setSpeed(200);
  dreapta.run(RELEASE);
}

void loop() {
  uint8_t i;

  stanga.run(FORWARD);
  dreapta.run(FORWARD);

  for (i = 0; i < 255; i++) {
    stanga.setSpeed(i);
    dreapta.setSpeed(i);

    delay(10);
  }

  for (i = 255; i != 0; i--) {
    stanga.setSpeed(i);
    dreapta.setSpeed(i);

    delay(10);
  }

  // Serial.print("tock");

  // motor1.run(BACKWARD);
  // motor2.run(BACKWARD);
  // motor3.run(BACKWARD);
  // motor4.run(BACKWARD);

  // for (i = 0; i < 255; i++) {
  //   motor1.setSpeed(i);
  //   motor2.setSpeed(i);
  //   motor3.setSpeed(i);
  //   motor4.setSpeed(i);
  //   delay(10);
  // }

  // for (i = 255; i != 0; i--) {
  //   motor1.setSpeed(i);
  //   motor2.setSpeed(i);
  //   motor3.setSpeed(i);
  //   motor4.setSpeed(i);

  //   delay(10);
  // }


  // Serial.print("tech");
  // motor1.run(RELEASE);
  // motor2.run(RELEASE);
  // motor3.run(RELEASE);
  // motor4.run(RELEASE);
  // delay(1000);
}
