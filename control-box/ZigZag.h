#include "Pattern.h"

#define ZIGZAG_WIDTH 8
#define ZIGZAG_SPEED_A 170
#define ZIGZAG_SPEED_B 32

/**
 * Sine wave (single factor plasma) that moves up the strip
 */
class ZigZag: public Pattern {
  public:
    void loop(byte fade)
    {
      CRGBPalette16 *palette = _state->getPalette();

      fill_solid(_state->leds, NUM_LEDS, CRGB::Black);

      int j = beat8(ZIGZAG_SPEED_A) * ZIGZAG_WIDTH / 256;
      int k = beat8(ZIGZAG_SPEED_B) * ZIGZAG_WIDTH / 256;

      for (int i = 0; i < NUM_LEDS; i++) {
        if (i % ZIGZAG_WIDTH == j) {
          _state->leds[i] = ColorFromPalette(*palette, 64);
        }
        if ((i-1) % ZIGZAG_WIDTH == j) {
          _state->leds[i] = ColorFromPalette(*palette, 192);
        }

        // if (i % ZIGZAG_WIDTH == (ZIGZAG_WIDTH - k)) {
        //   _state->leds[i] = ColorFromPalette(*palette, 192);
        // }
      }
    }
};
