#include "Pattern.h"

#define SWEEP_WIDTH 12
#define SWEEP_SPEED 23

/**
 * Sawtooth sweep
 */
class Sweep: public Pattern {
  public:
    void loop(byte fade)
    {
      CRGBPalette16 *palette = _state->getPalette();

      fill_solid(_state->leds, NUM_LEDS, CRGB::Black);

      int offset = triwave8(beat8(SWEEP_SPEED)) * (NUM_LEDS) / 256;

      for (int i = 0; i < NUM_LEDS; i++) {
        if (abs(offset - i) < SWEEP_WIDTH) {
          _state->leds[i] = ColorFromPalette(*palette, 128);
        }

        // if (i % ZIGZAG_WIDTH == (ZIGZAG_WIDTH - k)) {
        //   _state->leds[i] = ColorFromPalette(*palette, 192);
        // }
      }
    }
};
