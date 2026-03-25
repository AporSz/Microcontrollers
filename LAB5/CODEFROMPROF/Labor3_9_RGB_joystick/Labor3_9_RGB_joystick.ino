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


int analog_potmeter_value_x;
int analog_potmeter_value_y;
int scaled_value_x;
int scaled_value_y;

void setup() {
  // power-up safety delay
  delay(3000);
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(BRIGHTNESS);

  currentPalette = myRedWhiteBluePalette_p;
  currentBlending = LINEARBLEND;

  //set up the Serial port
  Serial.begin(9600);
}

void loop() {

  //read the potentiometer value
  analog_potmeter_value_x = analogRead(A0);
  analog_potmeter_value_y = analogRead(A1);

  Serial.print("Potentiometer values = ");
  Serial.print(analog_potmeter_value_x);
  Serial.print(",");
  Serial.print(analog_potmeter_value_y);

  //this is a value between 0 ... 1023
  //scale it to 0..7
  scaled_value_x = (int)round((analog_potmeter_value_x * 7.0 / 1023.0));
  scaled_value_y = (int)round((analog_potmeter_value_y * 7.0 / 1023.0));

  Serial.print(", Scaled values =  ");
  Serial.print(scaled_value_x);
  Serial.print(",");
  Serial.println(scaled_value_y);

  FillLEDs_in_matrix();

  FastLED.show();
  FastLED.delay(1000 / UPDATES_PER_SECOND);
}

void FillLEDs_in_matrix() {
  byte i, j;
  byte i_led;

  for (i = 0; i < 8; i++)
    for (j = 0; j < 8; j++) {
      led_matrix[i][j] = 0;
    }

  led_matrix[scaled_value_x][scaled_value_y] = 1;

  for (i = 0; i < 8; i++)
    for (j = 0; j < 8; j++) {
      i_led = i * 8 + j;
      leds[i_led] = ColorFromPalette(myRedWhiteBluePalette_p, 1, 255 * led_matrix[i][j], currentBlending);
    }
}

const TProgmemPalette16 myRedWhiteBluePalette_p PROGMEM = {
  CRGB::Red,
  CRGB::Gray,  // 'white' is too bright compared to red and blue
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
