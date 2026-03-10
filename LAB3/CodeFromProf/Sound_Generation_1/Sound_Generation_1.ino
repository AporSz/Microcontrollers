//1. this should generate now 5kHZ
//change it so it generates 10kHz, 15 kHz, 
//and then 20KHz ... can you still hear it?

//2. change the prescaler to either 256 or 1024
//generate a sound from a guitar sting 
//for example 196Hz

//256 prescaler: 16000000/256/196 is 318.87 half of that is 159.43
//so you need to go 160 steps  - meaning you start TCNTO = 255-160

//try to do the calculations for the 329.63 Hz guitar string
int main()
{
  
  //you can recalculate for a 16MHz clock signal

  //Prescaler Select: TCCR0B Bits 2,1,0
  // 0 0 0 No clock source
  // 0 0 1 Prescaler = 1 (no prescaler) 16 MHz
  // 0 1 0 Prescaler = 8             16MHz/8 = 2 MHz 2Mhz/256 = 62500 Hz
  // 0 1 1 Prescaler = 64               
  //1 0 0  Prescaler = 256              
  //1 0 1  Prescaler = 1024    
  cli();
   
  TCCR0A = 0b00000000;  //normal mode
  TCCR0B = 0b00000010;  //prescaler : 8

  TCNT0  = 0x00;       
  TIMSK0 = 0b00000000; //no interrupts
  TIFR0  = 0b00000001; //clear TOV flag
  //PORTD output pin
  DDRD |= 0b11100000;   //D7 D6 D5 outputs you can use any 

  byte pin_state = 0;    //state to flip back and forth

  sei();
  
  //main loop
  
  
  while (1)
    {

     
      while( TCNT0<0xFF);      
      TCNT0 = 55;

      //16 Mhz/ 8 = 2000 kHz is the time clock 
      //55 to 255 = 200 steps
      // 2000 kHz / 200 = 10 kHz 
      //sound needs up and down so  5 kHz is the sound frequency

      /*
      16 MHz / 8 (prescaler)/ 10000 Hz  (if this is what I want)/ 2 = 100 steps
      16 MHz / 8 (prescaler)/ 15000 Hz  (if this is what I want)/ 2 =  75 steps
      16 MHz / 8 (prescaler)/ 20000 Hz  (if this is what I want)/ 2 =  50 steps
      
       TCNT0 = 255-100;//255-75;//255-50;
      */

     
      //every overflow flips PORTD 7,6,5 outputs
      if (pin_state==0) 
        {
          PORTD &= 0b00011111;
          pin_state = 1;
        }
      else 
        {
          PORTD |= 0b11100000;
          pin_state = 0;
        }
    }

  
  return 0;  
}
