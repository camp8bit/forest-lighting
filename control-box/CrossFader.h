#include "Pattern.h"

/**
 * Takes two Patterns (or PatternLists) and allows crossfading between them
 */
class CrossFader : public Pattern {
  private:
    /**
     * The pattern to show when fade = 0
     */
    Pattern *_left;

    /**
     * The pattern to show when fade = 255
     */
    Pattern *_right;

  public:
    CrossFader(Pattern *left, Pattern *right) : _left(left), _right(right) {};

    void setup()
    {
      _left->setup();
      _right->setup();
    }

    void loop(byte fade)
    {
      if(fade < 255) _left->loop(255-fade);
      if(fade > 0) _right->loop(fade);

      CRGB *leftLEDs = _left->getLEDs();
      CRGB *rightLEDs = _right->getLEDs();
      
      switch(fade) {
        // If fade is 0 or 255 just to a straight copy of a single channel
        case 0:
          memcpy(_state->leds, leftLEDs, sizeof(CRGB) * NUM_LEDS);
          break;

        case 255:
          memcpy(_state->leds, rightLEDs, sizeof(CRGB) * NUM_LEDS);
          break;

        // Otherwise, mix channels
        default:
          for(byte i = 0; i < NUM_LEDS; i++) {
            _state->leds[i] = blend(leftLEDs[i], rightLEDs[i], fade);
          }
      }
   }
};
