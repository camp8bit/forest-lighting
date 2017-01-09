Creating Patterns
=================

To create new patterns, you should create a sub-class of Pattern.

Here is a simple example:

```c++
#include "Pattern.h"

class RandomPattern: public Pattern {
  public:
  	void setup()
  	{
  	}

    void loop(byte fade)
    {
      CRGBPalette16 *palette = _state->getPalette();

      for (int i = 0; i < NUM_LEDS; i++) {
        byte c = random(3) == 0 ? random(255) : CRGB::Black;
        byte colorindex = scale8( c, 200);
        _state->leds[i] = ColorFromPalette(*palette, colorindex);
      }
    }
};
```

There are two methods that you can override:

 * `setup()` This will be called whenever your pattern starts. (optional)
 * `loop(byte fade)` This will be called on ever frame when your pattern runs. `fade` is a
   variable that incidates how strongly activated your pattern is, for example, when fading
   a pattern in.

It makes use of variables in the `_state` object:

 * `NUM_LEDS` is the number of LEDs
 * `_state.leds` is an array of CRGB objects where you pattern should be written to
 * `_state.palette` is a palette you can make use of to have a palette of known colour

 In addition:

* `_state.activation` is a byte array that you can use to preserve other per-LED data


