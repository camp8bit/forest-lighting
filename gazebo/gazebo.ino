#include <FastLED.h>
// MSGEQ7 by NicoHood
#include <MSGEQ7.h>

#define LED_PIN     5
#define COLOR_ORDER GRB
#define CHIPSET     WS2812B

#define BRIGHTNESS  255
#define FRAMES_PER_SECOND 120
#define NUM_LEDS 250

#include "PatternState.h"
#include "PatternList.h"

#include "Plasma.h"
#include "Explosions.h"

#define MSGEQ7_MEASURE_PIN  A0
#define MSGEQ7_RESET_PIN    6
#define MSGEQ7_STROBE_PIN   4
#define MSGEQ7_SMOOTH       0
// around 900 seems like the fastest an empty loop with .Read() can do
#define MSGEQ7_INTERVAL     ReadsPerSecond(FRAMES_PER_SECOND)

CMSGEQ7<MSGEQ7_SMOOTH, MSGEQ7_RESET_PIN, MSGEQ7_STROBE_PIN, MSGEQ7_MEASURE_PIN> MSGEQ7;

Pattern *patterns[] = {  new Explosions() };
PatternList patternList(1, patterns);

PatternState state;

CRGBPalette16 bass_palette(CRGB::Black, CRGB(0,0,255), CRGB::DeepPink, CRGB::Purple);
CRGBPalette16 mid_palette(CRGB::Black, CRGB::Orange, CRGB::DarkRed, CRGB::Maroon);
CRGBPalette16 treble_palette(CRGB::Black, CRGB(85,85,85), CRGB(170,170,170), CRGB::White);

void setup() {
  Serial.begin(9600);
  Serial.println("gazebo 2017 - by camp8bit");
  Serial.print("compiled on ");
  Serial.println(__DATE__);

  MSGEQ7.begin();

  delay(500); // sanity delay
  FastLED.addLeds<CHIPSET, LED_PIN, COLOR_ORDER>(state.leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.setBrightness( BRIGHTNESS );
  FastLED.setMaxPowerInVoltsAndMilliamps(5, 600);

  // 3 blink boot indicator
  for (int i = 0; i < 4; ++i) {
    fill_solid(state.leds, NUM_LEDS, CRGB::DeepPink);
    FastLED.delay(20);
    fill_solid(state.leds, NUM_LEDS, CRGB::Black);
    FastLED.delay(50);
  }

  state.setPalette(&bass_palette);
  patternList.setState(&state);
  patternList.setup();
}

void loop()
{

  if(!MSGEQ7.read(MSGEQ7_INTERVAL)) {
    return;
  }

  uint8_t bass = mapNoise(MSGEQ7.get(MSGEQ7_LOW));
  uint8_t mid = mapNoise(MSGEQ7.get(MSGEQ7_MID));
  uint8_t treble = mapNoise(MSGEQ7.get(MSGEQ7_HIGH));
  uint8_t val = 0;
  if(bass > mid) {
    val = bass;
    state.setPalette(&bass_palette);
  } else if (mid > treble) {
      val = mid;
      state.setPalette(&mid_palette);
  } else {
    val = treble;
    state.setPalette(&treble_palette);
  }

  patternList.loop(val);

  FastLED.show(); // display this frame
}
