#include "Pattern.h"
#include <FastLED.h>

/**
 * Represents a list of patterns that you can switch between
 */
class PatternList: public Pattern {
  private:
    byte _curPattern;
    byte _numPatterns;
    Pattern **_patterns;

  public:
    PatternList(byte numPatterns, Pattern **patterns): _curPattern(0), _numPatterns(numPatterns), _patterns(patterns) {
    }

    void setup() {
      _patterns[_curPattern]->setup();
    }

    void loop(byte fade, byte volume) {
      _patterns[_curPattern]->loop(fade, volume);
    }

    void setState(PatternState *state)
    {
      _state = state;
      for(byte i = 0; i < _numPatterns; i++) {
        _patterns[i]->setState(state);
      }
    }

    /**
     * Switch to the next pattern
     */
    void next() {
      _curPattern = (_curPattern + 1) % _numPatterns;
      setup();
    }
};
