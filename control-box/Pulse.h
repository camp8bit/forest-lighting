#include "Pattern.h"

#define PULSE_SPEED 9
#define PULSE_DITHER 4

/**
 * Dithered solid pulse
 */

byte pulseCount;

class Pulse: public Pattern {
  public:
    void loop(byte fade)
    {
      CRGBPalette16 *palette = _state->getPalette();

      pulseCount++;

      byte b = sin8(beat8(PULSE_SPEED));

      fill_solid(_state->leds, NUM_LEDS, ColorFromPalette(*palette, b));

      for (int i = 0; i < NUM_LEDS; i++) {
        if (i % PULSE_DITHER == (pulseCount % PULSE_DITHER)) {
          _state->leds[i] = CRGB::Black;
        }
      }
    }
};
