#include "Pattern.h"

/**
 * Twinkle pattern
 */
class Twinkle: public Pattern {
  public:
    void loop(CRGB *leds, byte fade)
    {
      byte i;      
      for (i = 0; i < 2; i++) {
        leds[random(NUM_LEDS)] = ColorFromPalette(gPal, random(192));
      }
    }
};

