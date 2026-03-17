//www.elegoo.com
//2016.12.12

// define the LED digit patterns, from 0 - 9
// 1 = LED on, 0 = LED off, in this order:
//                74HC595 pin     Q0,Q1,Q2,Q3,Q4,Q5,Q6,Q7 
//                Mapping to      a,b,c,d,e,f,g of Seven-Segment LED
byte seven_seg_digits[10] = { B00000001,  // = 0
                              B00000010,  // = 1
                              B00000100,  // = 2
                              B00001000,  // = 3
                              B00010000,  // = 4
                              B00100000,  // = 5
                              B01000000,  // = 6
                              B10000000,  // = 7
                              B01000000,  // = 8
                              B00100000   // = 9
                             };
 
// connect to the ST_CP of 74HC595 (pin 3,latch pin) RCLK pin 12 on the chip
int latchPin = 3;
// connect to the SH_CP of 74HC595 (pin 4, clock pin) SRCLK pin 11 on the chip
int clockPin = 4;
// connect to the DS of 74HC595 (pin 2) SER (pin 14 on the chip)
int dataPin = 2;
 
//chip pin 14 SER needs to go digital pin 2 on the Arduino
//chip pin 13 nonOE needs to go to GND
//chip pin 12 RCLK goes to digital pin 3 on the Arduino
//chip pin 11 SRCLK goes to digital pin 4 on the Arduino
//chip pin 10 nonSRCLR goes to VCC
//chip pin 9 QH' does not need to be connected

void setup() {
  // Set latchPin, clockPin, dataPin as output
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
}
 
// display a number on the digital segment display
void sevenSegWrite(byte digit) {
  // set the latchPin to low potential, before sending data
  digitalWrite(latchPin, LOW);
     
  // the original data (bit pattern)
  shiftOut(dataPin, clockPin, LSBFIRST, seven_seg_digits[digit]);  
 
  // set the latchPin to high potential, after sending data
  digitalWrite(latchPin, HIGH);
}
 
void loop() {       

  for (byte digit = 0; digit < 10; digit++) {
    delay(100);
    sevenSegWrite(digit); 
  }
   
  // suspend 4 seconds
  delay(1000);
}
