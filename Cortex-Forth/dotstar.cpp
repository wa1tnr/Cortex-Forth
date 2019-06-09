// Sun Jun  9 01:26:44 UTC 2019

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
