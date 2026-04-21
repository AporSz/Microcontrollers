/*
Arduino 2x16 LCD - Detect Buttons
modified on 18 Feb 2019
by Saeed Hosseini @ Electropeak
https://electropeak.com/learn/
*/

#include <LiquidCrystal.h>
//LCD pin to Arduino
const int pin_RS = 8;
const int pin_EN = 9;
const int pin_d4 = 4;
const int pin_d5 = 5;
const int pin_d6 = 6;
const int pin_d7 = 7;
const int pin_BL = 10;

LiquidCrystal lcd(pin_RS, pin_EN, pin_d4, pin_d5, pin_d6, pin_d7);

int x = 0;
int y = 0;

void setup() {
  lcd.begin(16, 2);
  lcd.setCursor(0, 0);
  // lcd.print("Electropeak.com");
  // lcd.setCursor(0, 1);
  // lcd.print("Press Key:");
}

void loop() {
  int button;
  button = analogRead(0);
  // lcd.setCursor(10, 1);

  if (button < 60) {
    // lcd.print("Right ");
    x++;
    if (x > 15) x = 15;
  } else if (button < 200) {
    // lcd.print("Up    ");
    y = 0;
  } else if (button < 400) {
    // lcd.print("Down  ");
    y = 1;
  } else if (button < 600) {
    // lcd.print("Left  ");
    x--;
    if (x < 0) x = 0;
  } else if (button < 800) {
    // lcd.print("Select");
    x = 0;
    y = 0;
  }

  lcd.clear();
  lcd.setCursor(x, y);
  lcd.print("o");
  delay(100);
}