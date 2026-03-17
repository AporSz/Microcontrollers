
//PWM generation for laser module
//laser module setup: 
//    -    : GND
// middle  : VCC
// S(ignal): this is where the PWM signal goes ()

int direction = 1;

void setup() 
{
//I want to use Timer1
//FAST PWM mode on 10 bits so 0..1023 resolution

pinMode(9,OUTPUT);
pinMode(10,OUTPUT);

TCCR1A = 0b10100011; //10 10 toggle normal both for A and B x x and WGM 0111
TCCR1B = 0b00001010; // x x x 0 1 from WGMbol and 101 sets the /1024 prescaler
OCR1A = 0; //10 bit PWM goes from 0..1023 so this is middle, 50%
OCR1B = 900; // this sets the other duty cycle

//where are the OC1A and OC1B outputs?
//look at the pinout diagram for this
//this is visibly flashing this is not good
//fix it so the PWM frequency would be good
}

void loop() 
{
  
  // ... make it so it goes through the whole PWM range gradually
  // get brigther and then gets dimmer and so on

  //OCR1B++;

  if (direction == 1) {
    if (OCR1A < 1023) OCR1A++;
    else direction *= -1;
  }
  else {
    if (OCR1A > 0) OCR1A--;
    else direction *= -1;
  }

  delay(1);
}