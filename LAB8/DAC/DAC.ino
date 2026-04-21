#include <Wire.h>

#define MCP4725_ADDR 0x60
//twi address of the DAC unit we have
//MCP4725, by default (A0 pulled to GND)

int data;

void setup() {
  Wire.begin();
  Serial.begin(9600);
  Serial.println("TWI(I2C) DAC example code");
}

void loop() {

  Wire.beginTransmission(MCP4725_ADDR);  //addressing the I2C DAC unit with its SLA address
  Wire.write(64);                        // command to update the DAC directly (and ignore the EEPROM write)

  //12 bits means counting from 0...4095
  //LSB bit 5V/4095=0.00122 so it is 1.22 mV (milivolt)
  //data = 4095; //this should be 5V
  data = 2562;  //this should be 2.5V

  /*
  I want to get 3.14 V
  4095 5
  x  3.14

  3.14/5*4095=2571
*/

  Wire.write(data >> 4);
  Wire.write((data & 0x00FF) << 4);

  Wire.endTransmission();
  Serial.print("I sent to DAC this data = ");
  Serial.println(data, DEC);

  delay(1000);  //fast enough, 1s delays
}
