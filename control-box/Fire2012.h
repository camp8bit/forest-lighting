#include "Pattern.h"

#define COOLING  80
#define SPARKING 120

/**
 * Fire2012 pattern - copied from demo
 */
class Fire2012: public Pattern {
  public:
    void loop(byte fade)
    {
      // Array of temperature readings at each simulation cell
      static byte heat[NUM_LEDS];

      // Step 1.  Cool down every cell a little
      for ( int i = 0; i < NUM_LEDS; i++) {
        heat[i] = qsub8(heat[i],  random8(0, ((COOLING * 10) / NUM_LEDS) + 2));
      }

      // Step 2.  Heat from each cell drifts 'up' and diffuses a little
      for ( int k = NUM_LEDS - 1; k >= 2; k--) {
        heat[k] = (heat[k - 1] + heat[k - 2] + heat[k - 2] ) / 3;
      }

      // Step 3.  Randomly ignite new 'sparks' of heat near the bottom
      if ( random8() < SPARKING ) {
        int y = random8(7);
        heat[y] = qadd8(heat[y], random8(160, 255));
      }

      // Step 4.  Map from heat cells to LED colors
      for ( int j = 0; j < NUM_LEDS; j++) {
        // Scale the heat value from 0-255 down to 0-240
        // for best results with color palettes.
        byte colorindex = scale8(heat[j], 240);
        CRGB color = ColorFromPalette(_state->palette, colorindex);
        int pixelnumber;
        if (gReverseDirection) {
          pixelnumber = (NUM_LEDS - 1) - j;
        } else {
          pixelnumber = j;
        }
        _state->leds[pixelnumber] = color;
      }
    }
};
