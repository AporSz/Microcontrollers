//Low level code
//rewrite this to count in hexa a,b,c,d,e,F
//also rewrite it to count every second not every 0.1 seconds
//find the interrupt maybe there is a comment there

byte seg;
byte num[4];

//these hold the numbers
//PORTB BDATA: xxed.cgx
//PORTD DDATA: xafxxbxx

byte BDATA[16];
byte DDATA[16];

//the segment common cathodes
//segments: PB0,PD3,PD4,PD7;

void load_all_characters()
{
  //PORTB BDATA: xxed.cgx
  //PORTD DDATA: xafxxbxx

  BDATA[0] = 0b00110100;
  DDATA[0] = 0b01100100; 
  BDATA[1] = 0b00000100;
  DDATA[1] = 0b00000100; 
  BDATA[2] = 0b00110010;
  DDATA[2] = 0b01000100;
  BDATA[3] = 0b00010110;
  DDATA[3] = 0b01000100;
  BDATA[4] = 0b00000110;
  DDATA[4] = 0b00100100;
  BDATA[5] = 0b00010110;
  DDATA[5] = 0b01100000;
  BDATA[6] = 0b00110110;
  DDATA[6] = 0b01100000;
  BDATA[7] = 0b00000100;
  DDATA[7] = 0b01000100;
  BDATA[8] = 0b00110110;
  DDATA[8] = 0b01100100;
  BDATA[9] = 0b00010110;
  DDATA[9] = 0b01100100;
  // a
  BDATA[10] = 0b00110110;
  DDATA[10] = 0b01000100;
  // b
  BDATA[11] = 0b00110110;
  DDATA[11] = 0b00100000;
  //PORTB BDATA: xxed.cgx
  //PORTD DDATA: xafxxbxx
  // c
  BDATA[12] = 0b00110010;
  DDATA[12] = 0b00000000;
  // d
  BDATA[13] = 0b00110110;
  DDATA[13] = 0b00000100;
  // e
  BDATA[14] = 0b00110010;
  DDATA[14] = 0b01100000;
  // f
  BDATA[15] = 0b00100010;
  DDATA[15] = 0b01100000;

  /*
  BDATA[0] = 0b00ed.cg0;
  DDATA[0] = 0b0af00b00;
  */

}

void unset_number()
{
  PORTB &= ~0b00111110;
  PORTD &= ~0b01100100;
}

void set_number(byte character)
{
  PORTB |= BDATA[character];
  PORTD |= DDATA[character];
}

void unset_all_segments()
{
  PORTB |= 0b00000001; //seg 0: off
  PORTD |= 0b10011000;  //seg 1,2,3: off
}

void set_segment(byte seg_number)
{
switch(seg_number)  
  {
    case 0: {
            PORTB &= ~0b00000001; //seg 0: on
            PORTD |= 0b10011000;  //seg 1,2,3: off
            break;
            }
    case 1: {
            PORTD &= ~0b00001000; //seg 1: on 
            PORTD |= 0b10010000; //seg 2,3: off          
            PORTB |= 0b00000001;  //seg 0: off
            break;
            }
    case 2: {
            PORTD &= ~0b00010000; //seg 2: on
            PORTD |= 0b10001000; //seg 1,3: off          
            PORTB |= 0b00000001; //seg 0 off
            break;
            }
    case 3: {
            PORTD &= ~0b10000000; //seg 3: on
            PORTD |= 0b00011000;  //seg 1,2: off         
            PORTB |= 0b00000001; //seg 0 off
            break;
            }           
  } 
}

ISR(TIMER0_OVF_vect) 
{
  seg++;
  if (seg==4) seg=0;
  unset_all_segments();
  unset_number();
  set_number(num[seg]);
  set_segment(seg);
}

ISR(TIMER1_OVF_vect) 
{
  // count from 3035  to 65535 that is 62500 steps or 1s    counting
  // count from 59285 to 65535 that is 6250  steps or 0.1s  counting
  // count from 64910 to 65535 that is 625   steps or 0.01s counting
  TCNT1 = 3035; //59285; //3035; //64910;
  if (num[0]==15) 
    { 
      if (num[1]==15)
        {
          if (num[2]==15)
            {
            if (num[3]==15)
              {num[3]=0;num[2]=0;num[1]=0;num[0]=0;}
            else {num[3]++;num[2]=0;num[1]=0;num[0]=0;}
            }
          else
            {num[2]++;num[1]=0;num[0]=0;}
        }
      else {num[1]++;num[0]=0;}
    }
  else num[0]++;
}

int main()
{
  cli();

  //setup the output ports
  //pins 3..13 so PORTB0..5 
  //and PORTD2..7 are outputs

  DDRB |= 0b00111111;
  DDRD |= 0b11111100;

  //setup the two timers
  //TIMER0 will switch the segment
  //TIMER1 will count the seconds

  //TIMER0 the prescaler is the last 3 bits
  //of the TCCRB register
  //000 timer stopped
  //001 prescaler = 1
  //010 prescaler = 8
  //011 prescaler = 64
  //100 prescaler = 256
  //101 prescaler = 1024

  TCCR0A = 0b00000000;
  TCCR0B = 0b00000100;
  //16Mhz/256/256 = 244 Hz 
  //this is high enough for  
  //segment switching

  TCNT0 = 0x00;
  TIMSK0 = 0b00000001; 
  //we will have the OVF interrupt

  TCCR1A = 0b00000000;
  TCCR1B = 0b00000100; 
  //256 prescaler
  //16 MHz/256 = 62500 Hz
  // us period
  // 1s /  = 62500
  // count from 3035  to 65535 that is 62500 steps or 1s    counting
  // count from 59285 to 65535 that is 6250  steps or 0.1s  counting
  // count from 64910 to 65535 that is 625   steps or 0.01s counting
  TCNT1 = 3035; //59285;//3035; //64910;
  TIMSK1 = 0b00000001; 
  //we will have the OVF interrupt

  load_all_characters();
  unset_all_segments();
  num[0] = 0;
  num[1] = 0;
  num[2] = 0;
  num[3] = 0;
  seg = 0;

  sei();

  load_all_characters();
  unset_number();
  unset_all_segments();

  while(1)
  {
    //.. the main loop is free
  }  
    
  return 0;
}
