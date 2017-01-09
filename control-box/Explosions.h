#include "Pattern.h"

#define SPAWN_EXPLOSION 5
#define EXPLOSION_SIZE 30
#define NUM_EXPLOSIONS 10

struct explosion {
  bool active;
  int position;
  int size;
  CRGB color;
};

/**
 * Explosion pattern
 */
class Explosions: public Pattern {
  private:
    explosion _explosions[NUM_EXPLOSIONS];

  public:
    void setup()
    {
      for (int i=0; i < NUM_EXPLOSIONS; i++){
        _explosions[i].active = false;
      }
    }

    void loop(byte fade)
    {
      int i;
      int x;

      CRGBPalette16 *palette = _state->getPalette();

      if (random(100) < SPAWN_EXPLOSION) {
        int i = random(NUM_EXPLOSIONS - 1);
        _explosions[i].active = true;
        _explosions[i].size = 0;
        _explosions[i].position = random(NUM_LEDS);
        _explosions[i].color = (random(2) < 1) ? CRGB::Green : CRGB::Aqua;
      }

      for (i = 0; i < NUM_LEDS; i++) {
        _state->leds[i].fadeToBlackBy(16);
      }

      for (i=0; i < NUM_EXPLOSIONS; i++){
        if (!_explosions[i].active) {
          continue;
        }

        _explosions[i].size++;

        if (_explosions[i].size > EXPLOSION_SIZE) {
          _explosions[i].active = false;
          continue;
        }

        for (x = -_explosions[i].size / 2; x < _explosions[i].size / 2; x++) {
          int y = _explosions[i].position + x;

          if (y < 0 || y >= NUM_LEDS) {
            continue;
          }

          // use 240 instead of 255 to only use part of the palette
          byte modulus = (int) 240 - (240 * abs(x) / (_explosions[i].size / 2));
          _state->leds[y] = ColorFromPalette(*palette, modulus);
        }
      }
    }
};
