#include "Pattern.h"

/**
 * Sine wave (single factor plasma) that moves up the strip
 */
class Plasma: public Pattern {
  public:
    void loop(CRGB *leds, byte fade)
    {
      for (int i = 0; i < NUM_LEDS; i++) {
        byte c = sin8((long) i * 30 - millis() / 2);
        byte colorindex = scale8( c, 200);
        CRGB color = ColorFromPalette( gPal, colorindex);
        blend(CRGB::Black, color, fade);
      }
    }
};
