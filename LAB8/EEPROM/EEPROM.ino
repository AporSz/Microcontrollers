/* 
 Mikrokontrollers Lab 7 

 TWI using Wire.h 

*/

#include "Wire.h"

char test_text[100] = "my little pony\0";
uint8_t text_length;

byte EEPROM_address = 0x57;
byte i;
byte error;

byte data[100];
byte N;


void senddata() {
  text_length = strlen(test_text);
  Serial.print("Test text length = ");
  Serial.println(text_length);
  Serial.print("the test text is = ");
  for (int i = 0; i < text_length; i++) {
    Serial.print((char)test_text[i]);
    //Serial.print(" ");
  }
  Serial.println();

  Serial.println("Write this into the EEPROM");

  Wire.beginTransmission(EEPROM_address);
  Wire.write(0);  //address inside EEPROM
  Wire.write(0);  //the address is actually 2 bytes

  for (i = 0; i < text_length; i++) {
    Wire.write((char)test_text[i]);
    delay(10);
  }

  Wire.endTransmission();
  Serial.println("Text was written to EEPROM");
}

void receivedata() {

  byte i;

  //text_length = 6;

  Serial.println("Reading text from EEPROM");
  Wire.beginTransmission(EEPROM_address);

  Wire.write(0);  //  dummy write
  Wire.write(0);  // 2 bytes again
  error = Wire.endTransmission();
  delay(10);

  if (error != 0) Serial.println("Could not find this SLA address");
  else {
    //read the text
    Wire.beginTransmission(EEPROM_address);
    Wire.requestFrom(EEPROM_address, text_length);
    for (i = 0; i < text_length; i++) {
      data[i] = (char)Wire.read();
      delay(10);
    }
    Wire.endTransmission();

    //print it on serial
    Serial.print("What we got = ");
    for (i = 0; i < text_length; i++) {
      Serial.print((char)data[i]);
      //Serial.print(" ");
    }
    Serial.println();
  }
}

void setup() {
  Wire.begin();
  Serial.begin(9600);
  Serial.println("EEPROM read and write");
  text_length = strlen(test_text);
  senddata();
}



void loop() {
  receivedata();
  Serial.println("==");
  delay(2000);
}