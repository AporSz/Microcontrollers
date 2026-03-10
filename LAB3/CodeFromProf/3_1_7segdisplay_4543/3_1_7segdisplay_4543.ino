// Microcontrollers Lab 3
// 7 segment + 4543 chip demo

byte latch_disable_pin = 4; 
// 0 do not disable, latch enabled == keep last value 
// 1 disable, latch disabled == refresh based on current A,B,C,D

byte input_D_pin = 7;
byte input_C_pin = 5;
byte input_B_pin = 6;
byte input_A_pin = 8;

byte phase_pin = 9;      // 0 is common cathode 1 is common anode
byte blanking_pin = 10;  // 0 is no blanking 1 is blanking


void setup() 
{
  pinMode(latch_disable_pin,OUTPUT);
  pinMode(input_D_pin,OUTPUT);
  pinMode(input_C_pin,OUTPUT);
  pinMode(input_B_pin,OUTPUT);
  pinMode(input_A_pin,OUTPUT);
  pinMode(phase_pin,OUTPUT);
  pinMode(blanking_pin,OUTPUT);

  digitalWrite(latch_disable_pin,1); 
  //yes, disable latch
  digitalWrite(blanking_pin,0); 
  //no, do not blank display
  digitalWrite(phase_pin,1);
  //0 is common cathode, 1 is common anode 
  //it says SA on the common anode
  //it says SC on the common cathode
  //check what you have, modify accordingly

  digitalWrite(input_A_pin,0);
  digitalWrite(input_B_pin,0);
  digitalWrite(input_C_pin,0);
  digitalWrite(input_D_pin,0);
}

void loop() 
{
  // 0
  digitalWrite(input_A_pin,0);
  digitalWrite(input_B_pin,0);
  digitalWrite(input_C_pin,0);
  digitalWrite(input_D_pin,0);
  delay(1000);

  // 1
  digitalWrite(input_A_pin,1);
  digitalWrite(input_B_pin,0);
  digitalWrite(input_C_pin,0);
  digitalWrite(input_D_pin,0);
  delay(1000);

  // 2
  digitalWrite(input_A_pin,0);
  digitalWrite(input_B_pin,1);
  digitalWrite(input_C_pin,0);
  digitalWrite(input_D_pin,0);
  delay(1000);

  // 3
  digitalWrite(input_A_pin,1);
  digitalWrite(input_B_pin,1);
  digitalWrite(input_C_pin,0);
  digitalWrite(input_D_pin,0);
  delay(1000);

  // 4
  digitalWrite(input_A_pin,0);
  digitalWrite(input_B_pin,0);
  digitalWrite(input_C_pin,1);
  digitalWrite(input_D_pin,0);
  delay(1000);

  // 5
  digitalWrite(input_A_pin,1);
  digitalWrite(input_B_pin,0);
  digitalWrite(input_C_pin,1);
  digitalWrite(input_D_pin,0);
  delay(1000);

  // 6
  digitalWrite(input_A_pin,0);
  digitalWrite(input_B_pin,1);
  digitalWrite(input_C_pin,1);
  digitalWrite(input_D_pin,0);
  delay(1000);

  // 7
  digitalWrite(input_A_pin,1);
  digitalWrite(input_B_pin,1);
  digitalWrite(input_C_pin,1);
  digitalWrite(input_D_pin,0);
  delay(1000);

  // 8
  digitalWrite(input_A_pin,0);
  digitalWrite(input_B_pin,0);
  digitalWrite(input_C_pin,0);
  digitalWrite(input_D_pin,1);
  delay(1000);

  // 9
  digitalWrite(input_A_pin,1);
  digitalWrite(input_B_pin,0);
  digitalWrite(input_C_pin,0);
  digitalWrite(input_D_pin,1);

  // digitalWrite(blanking_pin,1); blanking 9

  delay(1000);
  // digitalWrite(blanking_pin,0); disabling blanking

  //complete the code so it would count from 0..9

  //use the latch disable to keep 5 for the 5..9 numbers
  //digitalWrite(latch_disable_pin,0);

  //use blanking to blank the 8 and 9
  //digitalWrite(blanking_pin,1);
}
