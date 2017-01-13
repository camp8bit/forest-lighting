#include "Pattern.h"

/**
 * Two factor plasma that shows a cropped segment oscillating up and down
 */
class PlasmaTwo: public Pattern {
  public:
    void loop(byte fade)
    {
      int i;
      CRGBPalette16 *palette = _state->getPalette();

      // Plasma pattern
      for (i = 0; i < NUM_LEDS; i++) {
        byte c = sin8((long) i * 31 - millis() / 3);
        byte b = sin8((long) i * 23 - millis() / 5);
        byte colorindex = scale8((b / 2 + c / 2), 200);
        _state->leds[i] = ColorFromPalette(*palette, colorindex);
      }

      // Crop to a segment that oscillates up and down
      int offset = triwave8(beat8(BREATHS_PER_MINUTE)) * (NUM_LEDS - STRIPE_WIDTH) / 256;

      for (i = 0; i < offset; i++) {
        _state->leds[i] = CRGB::Black;
      }

      for (i = offset + STRIPE_WIDTH; i < NUM_LEDS; i++) {
        _state->leds[i] = CRGB::Black;
      }
    }
};
