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
  protected:
    /**
     * A byte that can be used to track 'activation level' per LED
     * Patterns can use this as they wish
     */
    byte *activation;

    /**
     * A palette to for the patterns to use.
     */
    CRGBPalette16 *_palette;

  public:
    /**
     * The LEDs to write to
     */
    CRGB leds[NUM_LEDS];

    PatternState(): activation(0)
    {
      for(byte i = 0; i < NUM_LEDS; i++) {
        leds[i] = CRGB::Black;
      }
    };

    byte *getActivation()
    {
      // Allocate the byte array if it hasn't already been allocated
      if ((unsigned int)activation == 0) {
        activation = new byte[NUM_LEDS];
        for(byte i = 0; i < NUM_LEDS; i++) {
          activation[i] = 0;
        }
      }

      return activation;
    };

    void setPalette(CRGBPalette16 *palette)
    {
      _palette = palette;
    };

    CRGBPalette16 *getPalette()
    {
      return _palette;
    };

};

#endif
