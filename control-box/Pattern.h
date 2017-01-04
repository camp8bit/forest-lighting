#ifndef Pattern_h
#define Pattern_h

/**
 * Abstract base class for all patterns
 */

class Pattern {
  public:
    /**
     * Prepare the pattern to start running.
     * May be called multiple times, e.g. when switching between patterns
     * @param leds The LED strip to draw to
     */
    virtual void setup(CRGB *leds)
    {
    }
   
    /**
     * Run a single frame of the pattern
     * @param leds The LED strip to draw to
     * @param fade The activation level of this pattern. Use this to fade patterns in and out
     */
    virtual void loop(CRGB *leds, byte fade) = 0;
};

#endif
