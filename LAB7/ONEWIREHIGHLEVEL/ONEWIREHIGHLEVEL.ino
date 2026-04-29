#include <OneWire.h>            //library for 1W
#include <DallasTemperature.h>  //library for DS18B20

// 1W data line is on this
#define ONE_WIRE_BUS 2
// create an object that is responsible for handling the 1W bus
OneWire oneWire(ONE_WIRE_BUS);

// give this object to the thermometer library
DallasTemperature sensors(&oneWire);

void setup(void)
{
  //9600 baud
  Serial.begin(9600);
  Serial.println("One wire (1W) thermometer (DS18B20) high level example code");
  //initialize the thermometer
  sensors.begin();
}

void loop(void)
{

  //this is to write out in text
  // Serial.println("Reading values from the thermometer");
  // sensors.requestTemperatures(); 
  // Serial.println("Values we got:");

  // Serial.print("Temperature on the 0.th termometer: ");
  // Serial.println(sensors.getTempCByIndex(0));


  //this is for the seial plotter: 
  sensors.requestTemperatures();
  delay(100);
  Serial.println(sensors.getTempCByIndex(0));
  delay(100);

  //if there would be multiple thermometers, this is how we could read them:
  /*
  Serial.print("Temperature on the 1.st termometer: ");
  Serial.println(sensors.getTempCByIndex(1));
  */
  
}