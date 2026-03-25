
#include <FastLED.h>

#define LED_PIN     5
#define NUM_LEDS    24
#define BRIGHTNESS  64
#define LED_TYPE    WS2811
#define COLOR_ORDER GRB
CRGB leds[NUM_LEDS];

#define UPDATES_PER_SECOND 100

CRGBPalette16 currentPalette;
TBlendType    currentBlending;

extern CRGBPalette16 myRedWhiteBluePalette;
extern const TProgmemPalette16 myRedWhiteBluePalette_p PROGMEM;


int analog_potmeter_value; 
int scaled_value;

void setup()
{
  // power-up safety delay
  delay(3000); 
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.setBrightness(  BRIGHTNESS );
    
  currentPalette = myRedWhiteBluePalette_p;
  currentBlending = LINEARBLEND;
  
  //set up the Serial port
  Serial.begin(9600);
}
 
void loop()
{

  //read the potentiometer value  
  analog_potmeter_value = analogRead(A0);
  
  Serial.print("Potentiometer value = ");
  Serial.print(analog_potmeter_value);
  //this is a value between 0 ... 1023 
  //scale it to 0..24
  scaled_value = (int)round((analog_potmeter_value * 23.0/1023.0));

  Serial.print(", Scaled value =  ");
  Serial.println(scaled_value);
    
  static uint8_t startIndex = 0;
  startIndex = startIndex + 1; /* motion speed */
    
  FillLEDs_in_circle();
    
  FastLED.show();
  FastLED.delay(1000 / UPDATES_PER_SECOND);

}
 
void FillLEDs_in_circle()
{
    int i;

    for(i = 0; i < NUM_LEDS; i++) 
        {
            leds[i] = ColorFromPalette( currentPalette, i, 0 , currentBlending);
        }

    leds[scaled_value] = ColorFromPalette( currentPalette, scaled_value, 255 , currentBlending);

}

const TProgmemPalette16 myRedWhiteBluePalette_p PROGMEM =
{
    CRGB::Red,
    CRGB::Gray, // 'white' is too bright compared to red and blue
    CRGB::Blue,
    CRGB::Black,
    
    CRGB::Red,
    CRGB::Gray,
    CRGB::Blue,
    CRGB::Black,
    
    CRGB::Red,
    CRGB::Red,
    CRGB::Gray,
    CRGB::Gray,
    CRGB::Blue,
    CRGB::Blue,
    CRGB::Black,
    CRGB::Black
};
