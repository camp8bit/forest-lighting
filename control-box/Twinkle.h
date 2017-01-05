#include "Pattern.h"

/**
 * Calm twinkle pattern - like raindrops in a pond
 */
class Twinkle: public Pattern {
  public:
    void setup(CRGB *leds)
    {
      int i;

      for (i = 0; i < NUM_LEDS; i++) {
        gActivation[i] = 0;
      }
    }
    
    void loop(CRGB *leds, byte fade)
    {
      int i;

      // 3% chance of creating a droplet
      if(random(100) < 3) {
        gActivation[random(NUM_LEDS)] = 255;
      }

      byte colorRef;

      // Decay all activation levels
      for (i = 0; i < NUM_LEDS; i++) {
        if(gActivation[i] > 0) {
          gActivation[i]--;

          // Color bounces from black to white and back
          if(gActivation[i] > 127) {
            colorRef = (255-gActivation[i]) * 2;
          } else {
            colorRef = gActivation[i] * 2;
          }

          // Re-draw pixel
          leds[i] = blend(CRGB::Black, ColorFromPalette(gPal, colorRef), fade);
        }
      }

    }
};

