#include <OneWire.h>            //One wire library we will use this

// 1W data line is here
#define ONE_WIRE_BUS 2
// create an object that is responsible for the 1W line
OneWire oneWire(ONE_WIRE_BUS);

uint8_t presence_pulse;
uint8_t rom_address[8];
uint8_t data_read[8];

uint8_t found;

void setup() 
{
  //9600 baud serial comm
  Serial.begin(9600);
  Serial.println("Reading the One wire (1W) data bus");
}

void loop() 
{
uint8_t i;
float celsius;

  
  Serial.println("1W COmmunication START");
  //FIRST STEP: send a RESET pulse
  //everyone will answer this
  //if there was at least one answer we will see presence_pulse==1
  
  presence_pulse = oneWire.reset();
  
  Serial.print("After RESET we got a presence pulse : ");
  if (presence_pulse==1) Serial.println("Yes");
  else          Serial.println("No");
  delay(500);
  
  
  //SECOND STEP: addressing (ROM command)
  //only use one of the listed possibilities only comment out one
 
  //READ ROM command (to read rom we need to sent this: 0x33)
  //we can only use this if there is just ome device
  //and we want to read its address
  
  oneWire.write(0x33);   
  Serial.println("READ ROM command 0x33"); 
  Serial.print("ROM address that we read = ");
  for(i=0;i<8;i++) 
    {
      rom_address[i] = oneWire.read();
      Serial.print(" ");
      Serial.print(rom_address[i], HEX);
    } 
  Serial.println();
  delay(500);
  

  
  //SKIP ROM command (send this: 0xCC)
  //only use this if there is just one device
  //we do not wnat to address it, we skipo over this step
  /*
  oneWire.write(0xCC);
  */

 
  //MATCH ROM command (send this: 0x55)
  //we can use this even if there are multiple devices
  //we have to know the address to use this 
  /*
  Serial.println("I am matching the ROM address on the device");
  //here you have to put in the address you read previously
  rom_address[0] = 0x28;
  rom_address[1] = 0x55;
  rom_address[2] = 0xA2;
  rom_address[3] = 0x61; 
  rom_address[4] = 0x7;
  rom_address[5] = 0;
  rom_address[6] = 0;
  rom_address[7] = 0xE;
  
  oneWire.write(0x55);
  
  for(i=0;i<8;i++) 
    oneWire.write(rom_address[i]);
  */

  //SEARCH ROM command (send this: 0xF0)
  //if there are multiple devices and we cannot individually read them
  //we have to do a binary search on the existing addresses
  //if we use this the library will do the RESET for us
  /*
  found = oneWire.search(rom_address, false); 
  //false or true is about the alarm I think look up the library on github:
  //instead of 0xF0 it should be issuing command 0xEC

  if (found==1)
    {
      Serial.print("found ROM address where we have an ALARM set = ");
      for(i=0;i<8;i++) 
        {
          Serial.print(" ");
          Serial.print(rom_address[i], HEX);
        } 
      Serial.println();
    }
  else
    {
      //did not find new addresses
      Serial.println("I have not found new addresses");
      oneWire.reset_search();
    }

   */
  
  //THIRD STEP: command (RAM command)

  //READ POWER SUPPLY (send this: 0xB4)
  //we can read if the unit is parasitic or not
  /*
  oneWire.write(0xB4);
  uint8_t parasite_unit = oneWire.read_bit();
  if (parasite_unit==1)  Serial.println("Not parasitic powered");
  else Serial.println("Parasitic powered");
  delay(1000);
  */


  //WRITE SCRATCHPAD (send this: 0x4E)  
  /*
  oneWire.reset();
  oneWire.write(0xCC); //Skip ROM
  oneWire.write(0x4E); //write scratchpad
  oneWire.write(30); //29C T HIGH
  oneWire.write(21); //26C T LOW
  //oneWire.write(0b00011111); //configuration: 9 bits conversion 
   oneWire.write(0b00011111);
  //oneWire.write(0b01111111); //configuration: 12 bits conversion 
  Serial.println("I have written the configuration in the memory");
  delay(1000);
  */

  //after this, let's also write it to its EEPROM so it stays
  //COPY SCRATCHPAD (send this: 0x48)

  /*
  oneWire.reset();
  //oneWire.select(rom_address);
  oneWire.write(0xCC); //Skip ROM
  oneWire.write(0x48);
  Serial.println("I have written the configuration in the EEPROM");
  delay(2000);
  

  oneWire.reset();
  //oneWire.select(rom_address);
  oneWire.write(0xCC); //Skip ROM
  
  //let's read a temperature value
  //START CONVERSION command with strong pull-up (send this: 0x44)
  Serial.println("RAM Command: 0x44 convert temperature");
  Serial.println("There is a reading of a temperature we will give it 1 second to complete");
  oneWire.write(0x44);   
  //let's wait at least 750 ms here, I set ot to 1s
  delay(1000);

  
  //I need to read the value that now exists in the scracthpad
  //READ SCRATCHPAD parancs (send this: 0xBE)

  Serial.println("RESET");
  oneWire.reset();
  Serial.println("ROM command: SKIP ROM");
  oneWire.write(0xCC); //Skip ROM if I am using only one
  //oneWire.select(rom_address); //if there are more
  Serial.println("RAM command: Read Scratchpad");
  oneWire.write(0xBE);
  Serial.print("Raw data (scratchpad 9 byte) = ");
  for(i=0;i<9;i++) 
        {
          data_read[i] = oneWire.read();
          Serial.print(data_read[i], HEX);
          Serial.print(" ");
        }
  Serial.println();      
  Serial.print("CRC byte (we calculate this, it should match the last byte) = ");
  Serial.println(OneWire::crc8(data_read, 8), HEX);

  //alakítsuk át a hőmérsékletet
  
  uint8_t configuration_byte = data_read[4];// & 0x60;
 
  //we zero the bits that contain no data
  if (configuration_byte == 0x1F)  data_read[0] &= 0b11111000; // 9 bites 
  else if (configuration_byte == 0x3F)  data_read[0] &= 0b11111100; //10 bites
  else if (configuration_byte == 0x5F)  data_read[0] &= 0b11111110; //11 bites
 
  int16_t temperature_read = (data_read[1] << 8) | data_read[0];
  
  celsius = (float)temperature_read / 16.0;
  Serial.print("Temperature = ");
  Serial.print(celsius);
  Serial.println("C");

  Serial.println("configuration_bytes:");
  
  Serial.print("T HIGH = ");
  Serial.print(data_read[2]);
  Serial.println("C");

  Serial.print("T LOW = ");
  Serial.print(data_read[3]);
  Serial.println("C");

  Serial.print("Configuration byte = ");
  Serial.print(data_read[4],HEX);
  Serial.println();

  Serial.print("Resolution for the temperature conversion = ");
  if (configuration_byte == 0x1F)  Serial.println("9 bits");  
  else if (configuration_byte == 0x3F)  Serial.println("10 bits"); 
  else if (configuration_byte == 0x5F)  Serial.println("11 bits"); 
  else if (configuration_byte == 0x7F)  Serial.println("12 bits");
  
  */

  Serial.println(); 
  
  delay(200);

  
}
