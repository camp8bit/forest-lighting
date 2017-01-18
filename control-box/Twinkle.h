#include "Pattern.h"

/**
 * Calm twinkle pattern - like raindrops in a pond
 */
class Twinkle: public Pattern {
  public:
    void setup()
    {
      int i;
      byte *activation = _state->getActivation();

      for (i = 0; i < NUM_LEDS; i++) {
        activation[i] = 0;
      }
    }

    void loop(byte fade)
    {
      int i;
      byte *activation = _state->getActivation();
      CRGBPalette16 *palette = _state->getPalette();

      // 3% chance of creating a droplet
      if(random(100) < 3) {
        activation[random(NUM_LEDS)] = 255;
      }

      byte colorRef;

      // Decay all activation levels
      for (i = 0; i < NUM_LEDS; i++) {
        if(activation[i] > 0) {
          activation[i]--;
        }

        // Color bounces from black to white and back
        if(activation[i] > 127) {
          colorRef = (255-activation[i]) * 2;
        } else {
          colorRef = activation[i] * 2;
        }

        // Re-draw pixel
        _state->leds[i] = ColorFromPalette(*palette, colorRef);
      }

    }
};

