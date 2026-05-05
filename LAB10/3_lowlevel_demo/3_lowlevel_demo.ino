int sensorValue;
int lowbyte, highbyte;

double temperature;

void setup() {

  //Atmega328p.pdf a 29. chapter ADC 205. page

  /*
  ADMUX bits: REFS1 REFS0 ADLAR x MUX3 MUX2 MUX1 MUX0
  
  REFS1 REFS0
  
  0 0 AREF
  0 1 AVCC  5V reference
  1 0 Reserved
  1 1 Internal 1.1V

  ADLAR left adjust result

  MUX3 MUX2 MUX1 MUX0
  0 0 0 0  ADC0 this is the same as A0
  0 0 0 1  ADC1
  ...
  0 1 1 1  ADC7
  1 0 0 0  ADC8 (TEMP)
  ...

  ADCSRA bits: ADEN ADSC ADATE ADIF ADIE ADPS2 ADPS1 ADPS0

  ADEN - enable
  ADSC - start conversion
  ADATE - auto trigger enable
  ADIF - flag
  ADIE - interrupt enable
  ADPS2..0 - prescaler bitek

  speed is set:

  ADPS2 ADPS1 ADPS0
  0 0 0   /2 
  0 0 1   /2
  0 1 0   /4
  0 1 1   /8
  1 0 0   /16
  1 0 1   /32
  1 1 0   /64
  1 1 1   /128
 
 
*/

  //ADC init
  ADMUX = 0b01000000;

  ADMUX = 0b11001000;  // switch to internel thermometer
  //it will read a value that has a formula to convert in the atmega328p.pdf we will not do that now

  ADCSRA = 0b10000100;  // enable + /16 prescaler

  Serial.begin(9600);
}


void loop() {
  // instead of: sensorValue = analogRead(A0);
  ADCSRA |= 0b01000000;  //start conversion
  //ADCSRA |= 1<<ADSC;

  while (ADCSRA & 1 << ADIF)
    ;  //wait until complete

  lowbyte = ADCL;
  highbyte = ADCH;
  sensorValue = (highbyte << 8) | lowbyte;

  Serial.println(sensorValue);

  // measuring TEMP
  /*
  480 is 125 C
  352 is 25 C
  269 is -40C

  Atmel Atmega 328P 
  22.8.1. (216.page)
  */

  delay(200);
}
