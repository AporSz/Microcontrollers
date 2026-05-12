#include <HCSR04.h>  //by Martin Sosic
#include <LiquidCrystal_I2C.h>
#include <QTRSensors.h>

#include <AFMotor.h>

AF_DCMotor left_motor(1);
AF_DCMotor right_motor(2);

UltraSonicDistanceSensor right_sensor(19, 18);  //trigger white in 19 and echo 18 in green
UltraSonicDistanceSensor left_sensor(17, 16);   //trigger white in 17 and echo 16 in green
float right_distance, left_distance;

//line following
QTRSensors qtr;
const uint8_t SensorCount = 8;
uint16_t sensorValues[SensorCount];
byte sensorValue_dumb[SensorCount];

//0x27
//0x3F possible LCD TWIO addresses

LiquidCrystal_I2C lcd(0x27, 20, 4);
//LiquidCrystal_I2C lcd(0x3F,20,4);

uint8_t charmap[8];

void setup() {
  //LCD inti
  lcd.init();
  lcd.backlight();
  //new chars
  /*
   *   ***00000
   *   ***01010
   *   ***01010
   *   ***00000
   *   ***10001
   *   ***01110
   *   ***00000
   *   ***00000  */

  charmap[0] = 0b00000000;
  charmap[1] = 0b00000000;
  charmap[2] = 0b00000000;
  charmap[3] = 0b00000000;
  charmap[4] = 0b00000000;
  charmap[5] = 0b00000000;
  charmap[6] = 0b00000000;
  charmap[7] = 0b00011111;
  lcd.createChar(0, charmap);
  charmap[6] = 0b00011111;
  lcd.createChar(1, charmap);
  charmap[5] = 0b00011111;
  lcd.createChar(2, charmap);
  charmap[4] = 0b00011111;
  lcd.createChar(3, charmap);
  charmap[3] = 0b00011111;
  lcd.createChar(4, charmap);
  charmap[2] = 0b00011111;
  lcd.createChar(5, charmap);
  charmap[1] = 0b00011111;
  lcd.createChar(6, charmap);
  charmap[0] = 0b00011111;
  lcd.createChar(7, charmap);

  //soros kommunikacio
  Serial.begin(9600);
  Serial.println("Ultrahang Sensors");

  qtr.setTypeAnalog();
  qtr.setSensorPins((const uint8_t[]){ A15, A14, A13, A12, A11, A10, A9, A8 }, SensorCount);
  qtr.setEmitterPin(52);

  // turn on motor
  left_motor.setSpeed(200);
  left_motor.run(RELEASE);

  right_motor.setSpeed(200);
  right_motor.run(RELEASE);
}

void loop() {
  //kiolvassuk az ultrahang tavolsagszenzorokat
  right_distance = right_sensor.measureDistanceCm();
  left_distance = left_sensor.measureDistanceCm();

  //Serial.print(" left = ");
  // Serial.println(baloldali_tav);
  lcd.setCursor(0, 0);
  //lcd.print("bal = ");
  if (left_distance < 20) {
    lcd.print(left_distance, 2);
    lcd.print("   ");
    left_motor.run(FORWARD);
    left_motor.setSpeed(200);
    right_motor.run(RELEASE);
  } else {
    lcd.print("FREE");
    right_motor.run(FORWARD);
    right_motor.setSpeed(200);
  }
  delay(25);

  //Serial.print("jobb = ");
  //Serial.print(jobboldali_tav);
  lcd.setCursor(10, 0);
  //lcd.print("jobb = ");
  if (right_distance < 20) {
    lcd.print(right_distance, 2);
    lcd.print("   ");
    right_motor.run(FORWARD);
    right_motor.setSpeed(200);
    left_motor.run(RELEASE);
  } else {
    lcd.print("FREE");
    left_motor.run(FORWARD);
    left_motor.setSpeed(200);
  }
  delay(25);

  //beolvassuk a vonalkoveto szenzor ertekeket
  qtr.read(sensorValues);

  for (uint8_t i = 0; i < SensorCount; i++) {
    Serial.print(sensorValues[i]);
    Serial.print('\t');
  }
  Serial.println();

  for (uint8_t i = 0; i < SensorCount; i++) {
    // sersorvalue lebutitott megy 0..8ig sensorvalue megy 0..1024ig
    //400..950  -400   0..550     /550   0..1  x 8 0..8

    /*
    if (sensorValues[i]<400) sensorValue_lebutitott[i] = 0;
    else if (sensorValues[i]>950) sensorValue_lebutitott[i] = 7;
    else sensorValue_lebutitott[i] = (sensorValues[i]-400 ) / 69;
*/

    //sensor value with a lower resolution
    sensorValue_dumb[i] = sensorValues[i] / 128;

    Serial.print(sensorValue_dumb[i]);
    Serial.print('\t');
  }
  Serial.println();

  for (uint8_t i = 0; i < SensorCount; i++) {

    //show the IR results
    lcd.setCursor(2 + i * 2, 3);
    lcd.write(sensorValue_dumb[i]);
  }


  delay(10);
}
