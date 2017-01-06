#include "Pattern.h"

/**
 * Calm twinkle pattern - like raindrops in a pond
 */
class Twinkle: public Pattern {
  public:
    void setup()
    {
      int i;

      for (i = 0; i < NUM_LEDS; i++) {
        _state->activation[i] = 0;
      }
    }

    void loop(byte fade)
    {
      int i;

      // 3% chance of creating a droplet
      if(random(100) < 3) {
        _state->activation[random(NUM_LEDS)] = 255;
      }

      byte colorRef;

      // Decay all activation levels
      for (i = 0; i < NUM_LEDS; i++) {
        if(_state->activation[i] > 0) {
          _state->activation[i]--;

          // Color bounces from black to white and back
          if(_state->activation[i] > 127) {
            colorRef = (255-_state->activation[i]) * 2;
          } else {
            colorRef = _state->activation[i] * 2;
          }

          // Re-draw pixel
          _state->leds[i] = blend(CRGB::Black, ColorFromPalette(_state->palette, colorRef), fade);
        }
      }

    }
};

