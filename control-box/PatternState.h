#ifndef PatternState_h
#define PatternState_h

/**
 * Provides shared state between patterns.
 * State for a pattern can be memory intensive and with only 2KB of SRAM availble,
 * it's not practical to have separate state for each pattern.
 * 
 * Rather than use globals, we make use of a PatternState object.
 */
class PatternState {
  public:
    /**
     * The LEDs to write to
     */
    CRGB leds[NUM_LEDS];

    /**
     * A byte that can be used to track 'activation level' per LED
     * Patterns can use this as they wish
     */
    byte activation[NUM_LEDS];

    /**
     * A palette to for the patterns to use.
     */
    CRGBPalette16 palette;

};

#endif