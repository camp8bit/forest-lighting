#include "Pattern.h"

/**
 * Sine wave (single factor plasma) that moves up the strip
 */
class PlasmaDirectional: public Pattern {
  public:
    void loop(CRGB *leds, byte fade)
    {
      for (int i = 0; i < NUM_LEDS; i++) {
        long t = sin8((long) millis() / 17) * 2;
        t += sin8((long) millis() / 11) * 2;
        byte c = sin8((long) i * 22 + t);
        byte colorindex = scale8( c, 200);
        CRGB color = ColorFromPalette( gPal, colorindex);
        leds[i] = color;
        leds[i] %= fade;
      }
    }
};
