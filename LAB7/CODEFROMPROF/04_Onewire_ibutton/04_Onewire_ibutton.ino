#include <OneWire.h>            //One wire library we will use this

// 1W data line is here
#define ONE_WIRE_BUS 2

#define blue_pin 13
#define yellow_pin 12
#define green_pin 11
#define white_pin 10
#define red_pin 7

byte active_pin;

// create an object that is responsible for the 1W line
OneWire oneWire(ONE_WIRE_BUS);

uint8_t presence_pulse;
uint8_t rom_address[8];
uint8_t blue_data[8] = {0x01,0x68,0xD2,0x36,0x01,0x00,0x00,0x68};
uint8_t yellow_data[8] = {0x01,0x5A,0x65,0xC2,0x01,0x00,0x00,0x8A};
uint8_t green_data[8] = {0x01,0x1C,0xDF,0xC2,0x01,0x00,0x00,0xBF};
uint8_t black_data[8] = {0x01,0xC1,0xF9,0xBC,0x01,0x00,0x00,0xE8};

uint8_t found;

void setup() 
{
  pinMode(blue_pin, OUTPUT);
  pinMode(yellow_pin, OUTPUT);  
  pinMode(green_pin, OUTPUT);
  pinMode(white_pin, OUTPUT);
  pinMode(red_pin, OUTPUT);

  digitalWrite(blue_pin,0);
  digitalWrite(yellow_pin,0);  
  digitalWrite(green_pin,0);
  digitalWrite(white_pin,0);
  digitalWrite(red_pin,0);
  
  //9600 baud serial comm
  Serial.begin(9600);
  Serial.println("One wire buttons");
}

void compare_addresses()
{

byte i;
bool match_blue;
bool match_yellow;
bool match_green;
bool match_black;

match_blue = true;
match_yellow = true;
match_green = true;
match_black = true;

for(i=0;i<8;i++) 
        {
          if (rom_address[i]!=blue_data[i]) match_blue = false;
          if (rom_address[i]!=yellow_data[i]) match_yellow = false;
          if (rom_address[i]!=green_data[i]) match_green = false;
          if (rom_address[i]!=black_data[i]) match_black = false;
        }

digitalWrite(active_pin,0);

active_pin = red_pin;
if (match_blue) active_pin = blue_pin;
if (match_yellow) active_pin = yellow_pin;
if (match_green) active_pin = green_pin;
if (match_black) active_pin = white_pin;
digitalWrite(active_pin,1);

}


void loop() 
{
  byte i;

  //Serial.println("1W Communication START");
  //FIRST STEP: send a RESET pulse
  //everyone will answer this
  //if there was at least one answer we will see presence_pulse==1
  
  presence_pulse = oneWire.reset();
  
  
  //Serial.print("After RESET we got a presence pulse : ");
  //if (presence_pulse==1) Serial.println("Yes");
  //else          Serial.println("No");
  
  delay(500);
  
  
  //SECOND STEP: addressing (ROM command)
  //only use one of the listed possibilities only comment out one
 
  //READ ROM command (to read rom we need to sent this: 0x33)
  //we can only use this if there is just ome device
  //and we want to read its address
  

   if (presence_pulse==1)
   {
      oneWire.write(0x33);   
      //Serial.println("READ ROM command 0x33"); 
      Serial.print("ROM address that we read = ");
      for(i=0;i<8;i++) 
        {
          rom_address[i] = oneWire.read();
          Serial.print(" ");
          Serial.print(rom_address[i], HEX);
        } 
      Serial.println();

      compare_addresses();
      delay(500);
   }

    delay(500);
  
}
