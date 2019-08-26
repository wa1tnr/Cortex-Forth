// Sun Jun  9 01:26:44 UTC 2019
#include "compatibility.h"
#ifdef HAS_DOTSTAR_LIB
#include <Arduino.h>
#include <Adafruit_DotStar.h>
#define NUMPIXELS 1 // Number of LEDs in strip

/*

dotstar pins

  ItsyBitsy:
  PB02 clock D6
  PB03 data  D8

  Trinket Mo:
  PA01 clock D8
  PA00 data D7

*/

#ifdef ADAFRUIT_GEMMA_M0

  #undef DATAPIN
  #undef CLOCKPIN

  #define DATAPIN    3
  #define CLOCKPIN   4

#endif // #ifdef ADAFRUIT_GEMMA_M0

#ifdef ADAFRUIT_TRINKET_M0

  #undef DATAPIN
  #undef CLOCKPIN

  #define DATAPIN    7
  #define CLOCKPIN   8

#endif // #ifdef ADAFRUIT_TRINKET_M0

#ifdef ADAFRUIT_ITSYBITSY_M4_EXPRESS

  #undef DATAPIN
  #undef CLOCKPIN

  #define DATAPIN    8
  #define CLOCKPIN   6

#endif // #ifdef ADAFRUIT_ITSYBITSY_M4_EXPRESS

Adafruit_DotStar strip = Adafruit_DotStar(
  NUMPIXELS, DATAPIN, CLOCKPIN, DOTSTAR_BRG);

void setup_dotstar(void) {
  strip.begin(); // Initialize pins for output
  strip.show();  // Turn all LEDs off ASAP
}

uint16_t red = 0, green = 0, blue = 0;

void clear_dotStarColors(void) {
  red = 0; green = 0; blue = 0; // black/no colors/no light at all
  strip.setPixelColor(0, blue, red, green); // Set the pixel colors
  strip.show();              // Refresh strip
}

void set_dotStarColors(void) {
  red = 0; green = 22; blue = 22; // cyan
  strip.setPixelColor(0, blue, red, green);
  strip.show();
}

void wiggleDotStarOnce(void) { // toggle at a rate
  set_dotStarColors();
  delay(200);
  clear_dotStarColors();
  delay(200);
}

void wiggleDSManyTimes(void) { // repeat the toggle
  for (int i = 8; i>0; i--) {
    wiggleDotStarOnce();
  }
}
#endif // HAS_DOTSTAR_LIB
