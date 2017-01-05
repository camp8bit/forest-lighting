#include "Pattern.h"

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

    void setup(CRGB *leds) {
      _patterns[_curPattern]->setup(leds);
    }
    
    void loop(CRGB *leds, byte fade) {
      _patterns[_curPattern]->loop(leds, fade);
    }

    /**
     * Switch to the next pattern
     */
    void next() {
      _curPattern = (_curPattern + 1) % _numPatterns;
      setup(leds); // to do: remove reference to global
    }
};
