//We always have to include the library
#include "LedControl.h"

/*
 Now we need a LedControl to work with.
 ***** These pin numbers will probably not work with your hardware *****
 pin 12 is connected to the DataIn 
 pin 11 is connected to the CLK 
 pin 10 is connected to LOAD 
 We have only a single MAX72XX.
 */
LedControl lc=LedControl(12,11,10,1);



/* we always wait a bit between updates of the display */
unsigned long delaytime=1000;

void setup() {
  /*
   The MAX72XX is in power-saving mode on startup,
   we have to do a wakeup call
   */
  lc.shutdown(0,false);
  /* Set the brightness to a medium values */
  lc.setIntensity(0,8);
  /* and clear the display */
  lc.clearDisplay(0);

  birthday();
}


/*
 This method will display the characters for the
 word "Arduino" one after the other on digit 0. 
 */
void writeArduinoOn7Segment() {
  //a
  lc.setChar(0,0,'a',false);
  delay(delaytime);
  //r
  lc.setRow(0,0,0x05);
  delay(delaytime);
  //d
  lc.setChar(0,0,'d',false);
  delay(delaytime);
  //u
  lc.setRow(0,0,0x1c);
  delay(delaytime);
  //i
  lc.setRow(0,0,B00010000);
  delay(delaytime);
  //n
  lc.setRow(0,0,0x15);
  delay(delaytime);
  //o
  lc.setRow(0,0,0x1D);
  delay(delaytime);
  //CLEAR
  lc.clearDisplay(0);
  delay(delaytime);
} 

/*
  This method will scroll all the hexa-decimal
 numbers and letters on the display. You will need at least
 four 7-Segment digits. otherwise it won't really look that good.
 */
void scrollDigits() {
  for(int i=0;i<13;i++) {
    lc.setDigit(0,3,i,false);
    lc.setDigit(0,2,i+1,false);
    lc.setDigit(0,1,i+2,false);
    lc.setDigit(0,0,i+3,false);
    delay(delaytime);
  }
  lc.clearDisplay(0);
  delay(delaytime);
}

void birthday() {
  lc.setDigit(0, 7, 2, false);
  lc.setDigit(0, 6, 0, false);
  lc.setDigit(0, 5, 0, false);
  lc.setDigit(0, 4, 2, false);
  lc.setDigit(0, 3, 0, false);
  lc.setDigit(0, 2, 6, false);
  lc.setDigit(0, 1, 1, false);
  lc.setDigit(0, 0, 6, false);
}

void loop() { 
  // writeArduinoOn7Segment();
  // scrollDigits();
}
