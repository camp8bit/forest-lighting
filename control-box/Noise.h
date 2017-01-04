#include "Pattern.h"

/**
 * Noise
 */
class Noise: public Pattern {
  public:
    void loop(CRGB *leds, byte fade)
    {
      for (int i = 0; i < NUM_LEDS; i++) {
        byte c = random(3) == 0 ? random(255) : CRGB::Black;
        byte colorindex = scale8( c, 200);
        CRGB color = ColorFromPalette( gPal, colorindex);
        leds[i] = blend(CRGB::Black, color, fade);
      }
    }
};
