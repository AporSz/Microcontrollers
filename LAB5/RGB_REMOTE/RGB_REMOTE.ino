#include <Arduino.h>
#include "PinDefinitionsAndMore.h"
#include <IRremote.hpp>

#include <FastLED.h>

#define LED_PIN 5
#define NUM_LEDS 64
#define BRIGHTNESS 64
#define LED_TYPE WS2811
#define COLOR_ORDER GRB
CRGB leds[NUM_LEDS];

byte led_matrix[8][8];

#define UPDATES_PER_SECOND 100

CRGBPalette16 currentPalette;
TBlendType currentBlending;

extern CRGBPalette16 myRedWhiteBluePalette;
extern const TProgmemPalette16 myRedWhiteBluePalette_p PROGMEM;

#include <Arduino.h>
//Define macros for input and output pin etc.
#include "PinDefinitionsAndMore.h"
#include <IRremote.hpp>

void setup() {
  // power-up safety delay
  delay(3000);
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(BRIGHTNESS);

  currentPalette = myRedWhiteBluePalette_p;
  currentBlending = LINEARBLEND;

  //set up the Serial port
  Serial.begin(9600);

  IrReceiver.begin(3, ENABLE_LED_FEEDBACK, USE_DEFAULT_FEEDBACK_LED_PIN);
  Serial.print(F("Ready to receive Color Buttons  "));
  Serial.print(F("at pin "));
  Serial.println("3");
}

void loop() {

  if (IrReceiver.decode()) {

    IrReceiver.resume();  // Enable receiving of the next value

    if (IrReceiver.decodedIRData.command == 0x37) {
      Serial.println("RED");
      FillLEDs_in_matrix(0);
      FastLED.show();
      //FastLED.delay(1000 / UPDATES_PER_SECOND);
    } else if (IrReceiver.decodedIRData.command == 0x36) {
      Serial.println("GREEN");
      FillLEDs_in_matrix(1);
      FastLED.show();
      //FastLED.delay(1000 / UPDATES_PER_SECOND);
    } else if (IrReceiver.decodedIRData.command == 0x32) {
      Serial.println("YELLOW");
      FillLEDs_in_matrix(2);
      FastLED.show();
      //FastLED.delay(1000 / UPDATES_PER_SECOND);
    } else if (IrReceiver.decodedIRData.command == 0x34) {
      Serial.println("BLUE");
      FillLEDs_in_matrix(3);
      FastLED.show();
      //FastLED.delay(1000 / UPDATES_PER_SECOND);
    }
  }


  //FillLEDs_in_matrix();

  //FastLED.show();
  //FastLED.delay(1000 / UPDATES_PER_SECOND);
}

void FillLEDs_in_matrix(byte color) {
  byte i_led;

  for (i_led = 0; i_led < NUM_LEDS; i_led++)
    leds[i_led] = ColorFromPalette(myRedWhiteBluePalette_p, color * 16, 255, currentBlending);

  delay(250);
}

const TProgmemPalette16 myRedWhiteBluePalette_p PROGMEM = {
  CRGB::Red,
  CRGB::Green,
  CRGB::Yellow,
  CRGB::Blue,

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
