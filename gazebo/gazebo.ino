#include <FastLED.h>
// MSGEQ7 by NicoHood
#include <MSGEQ7.h>

#define LED_PIN     5
#define COLOR_ORDER GRB
#define CHIPSET     WS2812B

#define BRIGHTNESS  255
#define FRAMES_PER_SECOND 120
#define NUM_LEDS 7

CRGB leds[NUM_LEDS];

#define MSGEQ7_MEASURE_PIN  A0
#define MSGEQ7_RESET_PIN    6
#define MSGEQ7_STROBE_PIN   4
#define MSGEQ7_SMOOTH       0
// around 900 seems like the fastest an empty loop with .Read() can do
#define MSGEQ7_INTERVAL     ReadsPerSecond(FRAMES_PER_SECOND)

CMSGEQ7<MSGEQ7_SMOOTH, MSGEQ7_RESET_PIN, MSGEQ7_STROBE_PIN, MSGEQ7_MEASURE_PIN> MSGEQ7;

void setup() {
  Serial.begin(9600);
  Serial.println("gazebo 2017 - by camp8bit");
  Serial.print("compiled on ");
  Serial.println(__DATE__);
  delay(500); // sanity delay
  FastLED.addLeds<CHIPSET, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.setBrightness( BRIGHTNESS );
  FastLED.setMaxPowerInVoltsAndMilliamps(5, 600);

  MSGEQ7.begin();

  // 3 blink boot indicator
  for (int i = 0; i < 4; ++i) {
    fill_solid(leds, NUM_LEDS, CRGB::DeepPink);
    FastLED.delay(20);
    fill_solid(leds, NUM_LEDS, CRGB::Black);
    FastLED.delay(50);
  }

  FastLED.delay(500); // sanity delay
}

void loop()
{

  if(!MSGEQ7.read(MSGEQ7_INTERVAL)) {
    return;
  }

  uint8_t val = MSGEQ7.get(MSGEQ7_LOW);
  // Reduce noise
  val = mapNoise(val);
  // Visualize leds to the beat
  CRGB color = CRGB::Blue;
  color.nscale8_video(val);
  fill_solid(leds, NUM_LEDS, color);
  FastLED.show(); // display this frame
}
