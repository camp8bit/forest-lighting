#include <FastLED.h>

#define LED_PIN     3
#define COLOR_ORDER GRB
#define CHIPSET     WS2812B
#define NUM_LEDS    90
#define PIR_PIN     5

#define BRIGHTNESS  255
#define FRAMES_PER_SECOND 120
#define STRIPE_WIDTH 30

#define BREATHS_PER_MINUTE 15
#define HEARTBEAT 70

#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))

bool gReverseDirection = false;

// The actual LEDs sent to the neopixel
CRGB leds[NUM_LEDS];

// Activation values that can be used as an intermediary
byte gActivation[NUM_LEDS];


CRGBPalette16 gPal;

// Last time sensor was triggered
unsigned long lastTriggered;

// First trigger of sequence
unsigned long firstTriggered;

#include "Twinkle.h"

#include "Plasma.h"
#include "PlasmaTwo.h"
#include "PlasmaDirectional.h"
#include "Fire2012.h"
#include "Explosions.h"
#include "Noise.h"

// Configure the patterns available
Pattern *wakePatterns[] = { new Explosions(), new Fire2012(), new Plasma(), new PlasmaTwo(), new PlasmaDirectional(), new Noise() };
Pattern *sleepPatterns[] = { new Twinkle() };

byte curWakePattern = 0;
byte curSleepPattern = 0;

void setup() {
  Serial.begin(9600);
  Serial.println("control-box 2017 - by camp8bit");
  Serial.print("compiled on ");
  Serial.println(__DATE__);

  delay(500); // sanity delay
  FastLED.addLeds<CHIPSET, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.setBrightness( BRIGHTNESS );

  FastLED.setMaxPowerInVoltsAndMilliamps(5, 600);

  pinMode(PIR_PIN, INPUT);

  lastTriggered = millis();

  firstTriggered = millis();

  // This first palette is the basic 'black body radiation' colors,
  // which run from black to red to bright yellow to white.
  gPal = HeatColors_p;

  // These are other ways to set up the color palette for the 'fire'.
  // First, a gradient from black to red to yellow to white -- similar to HeatColors_p
  //   gPal = CRGBPalette16( CRGB::Black, CRGB::Red, CRGB::Yellow, CRGB::White);

  // Second, this palette is like the heat colors, but blue/aqua instead of red/yellow
  // gPal = CRGBPalette16( CRGB::Black, CRGB::Blue, CRGB::Aqua,  CRGB::White);
  gPal = CRGBPalette16( CRGB::Black, CRGB::FireBrick, CRGB::DeepPink,  CRGB::White);
  // gPal = CRGBPalette16( CRGB::Black, CRGB(0,0,255), CRGB::White);
  // gPal = CRGBPalette16( CRGB::Black, CRGB::ForestGreen, CRGB::Yellow,  CRGB::White);
  // gPal = CRGBPalette16( CRGB::Black, CRGB::Red, CRGB::Pink,  CRGB::White);

  // Third, here's a simpler, three-step gradient, from black to red to white
  //   gPal = CRGBPalette16( CRGB::Black, CRGB::Red, CRGB::White);

  wakePatterns[curWakePattern]->setup(leds);
  sleepPatterns[curSleepPattern]->setup(leds);
}

// ms of fade up
#define FADE_UP 2000
#define MIN_PERIOD (FADE_UP + 2000)
#define FADE_DOWN 2000

void loop()
{
  // Add entropy to random number generator; we use a lot of it.
  random16_add_entropy( random());

  // Fourth, the most sophisticated: this one sets up a new palette every
  // time through the loop, based on a hue that changes every time.
  // The palette is a gradient from black, to a dark color based on the hue,
  // to a light color based on the hue, to white.
  //
  //   static uint8_t hue = 0;
  //   hue++;
  //   CRGB darkcolor  = CHSV(hue,255,192); // pure hue, three-quarters brightness
  //   CRGB lightcolor = CHSV(hue,128,255); // half 'whitened', full brightness
  //   gPal = CRGBPalette16( CRGB::Black, darkcolor, lightcolor, CRGB::White);

  // Fading
  //
  // This section handles fading the Pattern in and out depending on when the
  // motion sensor was first triggered, and most recently triggered. We have the
  // motion sensors set for minimum cycle time, so they have a period of about
  // 1-2 seconds. We fade the lights up when someone enters the scene, then keep
  // them faded up for a minimum period. If the PIR triggers again in this time
  // we keep the lights up. The goal is to have smooth transitions when someone
  // walks up, and if people stand around under the lamp posts (and trigger the
  // PIR every few seconds), the lamp posts will stay  lit until the person
  // walks away for good.

  if (digitalRead(PIR_PIN) == HIGH) {
    if (millis() > lastTriggered + MIN_PERIOD + FADE_DOWN ) {
      firstTriggered = millis();
    }

    lastTriggered = millis();
  }

  float fade = 0;

  // Use longs for your time math or you'll have cool bugs you can't work out
  long dt;
  unsigned long t = millis();

  if (t - firstTriggered <= FADE_UP) {
    // fading up
    dt = t - firstTriggered;
    fade = 255.0 / FADE_UP * dt;
  } else if (t - lastTriggered <= MIN_PERIOD) {
    // staying up
    fade = 255.0;
  } else if (t - lastTriggered < MIN_PERIOD + FADE_DOWN) {
    dt = t - lastTriggered - MIN_PERIOD;
    fade = 255.0 - (255.0 / FADE_DOWN * dt);

    // Cycle to the next Pattern
    // To do: make this more robust
    if(fade < 2) nextWakePattern();

  } else {

    fade = 0;
  }

  byte bF = max(0, min(255, fade));

  if (bF > 0) {
    wakePatterns[curWakePattern]->loop(leds, bF);
  }
  if (bF < 64) {
    sleepPatterns[curSleepPattern]->loop(leds, bF);
  }

  FastLED.show(); // display this frame
  FastLED.delay(1000 / FRAMES_PER_SECOND);
}

/**
 * Skip to the next awake Pattern
 */
void nextWakePattern()
{
  curWakePattern = (curWakePattern + 1) % ARRAY_SIZE(wakePatterns);  

  Serial.print("nextWakePattern: ");
  Serial.print(curWakePattern);
  Serial.print(" / ");
  Serial.println(ARRAY_SIZE(wakePatterns));

  wakePatterns[curWakePattern]->setup(leds);
}

/**
 * Skip to the next awake Pattern
 */
void nextSleepPattern()
{
  curSleepPattern = (curSleepPattern + 1) % ARRAY_SIZE(sleepPatterns);  
  sleepPatterns[curSleepPattern]->setup(leds);
}


