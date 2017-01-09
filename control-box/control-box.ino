#include <FastLED.h>

//#define DEV_MODE

#define LED_PIN     3
#define COLOR_ORDER GRB
#define CHIPSET     WS2812B

#define BRIGHTNESS  255
#define FRAMES_PER_SECOND 120
#define STRIPE_WIDTH 30

#define BREATHS_PER_MINUTE 15
#define HEARTBEAT 70

// ms of fade up
#define FADE_UP 5000
#define MIN_PERIOD (FADE_UP + 15000)
#define FADE_DOWN 10000

#ifdef DEV_MODE
  #define NUM_LEDS    60
  #define PIR_PIN     (new PullupPinSensor(6))
  #define DEBUG_PRINT(msg) (Serial.println(msg))

#else
  #define NUM_LEDS    90
  #define PIR_PIN     (new PinSensor(5))
  #define DEBUG_PRINT(msg) ()
#endif

// Activation values that can be used as an intermediary

#include "PinSensor.h"
#include "ASRFader.h"

#include "PatternState.h"
#include "PatternList.h"
#include "CrossFader.h"

#include "Twinkle.h"
#include "Plasma.h"
#include "PlasmaTwo.h"
#include "PlasmaDirectional.h"
#include "Fire2012.h"
#include "Explosions.h"
#include "Noise.h"

// Fader control will fade the lights up when someone enters the scene, then keep
// them faded up for a minimum period. If the PIR triggers again in this time
// we keep the lights up. The goal is to have smooth transitions when someone
// walks up, and if people stand around under the lamp posts (and trigger the
// PIR every few seconds), the lamp posts will stay  lit until the person
// walks away for good.
ASRFader fadeControl(PIR_PIN, FADE_UP, MIN_PERIOD, FADE_DOWN);


// Configure the patterns available
Pattern *wakePatternItems[] = { new Explosions(), new Fire2012(), new Plasma(), new PlasmaTwo(), new PlasmaDirectional(), new Noise() };
PatternList wakePatterns(6, wakePatternItems);

Pattern *sleepPatternItems[] = { new Twinkle() };
PatternList sleepPatterns(1, sleepPatternItems);

// Create a crossfader for blending between sleep and wake
CrossFader crossfader(&sleepPatterns, &wakePatterns);

PatternState state, sleepState, wakeState;

void setup() {
  Serial.begin(9600);
  Serial.println("control-box 2017 - by camp8bit");
  Serial.print("compiled on ");
  Serial.println(__DATE__);

  delay(500); // sanity delay
  FastLED.addLeds<CHIPSET, LED_PIN, COLOR_ORDER>(state.leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.setBrightness( BRIGHTNESS );

  FastLED.setMaxPowerInVoltsAndMilliamps(5, 600);

  // This first palette is the basic 'black body radiation' colors,
  // which run from black to red to bright yellow to white.

  // These are other ways to set up the color palette for the 'fire'.
  // First, a gradient from black to red to yellow to white -- similar to HeatColors_p
  //   state->palette = CRGBPalette16( CRGB::Black, CRGB::Red, CRGB::Yellow, CRGB::White);

  // Second, this palette is like the heat colors, but blue/aqua instead of red/yellow
  // state->palette = CRGBPalette16( CRGB::Black, CRGB::Blue, CRGB::Aqua,  CRGB::White);
  sleepState.palette = wakeState.palette = CRGBPalette16( CRGB::Black, CRGB(0,0,255), CRGB(0,255,255), CRGB::White);
  // state->palette = CRGBPalette16( CRGB::Black, CRGB(0,0,255), CRGB::White);
  // state->palette = CRGBPalette16( CRGB::Black, CRGB::ForestGreen, CRGB::Yellow,  CRGB::White);
  // state->palette = CRGBPalette16( CRGB::Black, CRGB::Red, CRGB::Pink,  CRGB::White);

  // Third, here's a simpler, three-step gradient, from black to red to white
  //   state->palette = CRGBPalette16( CRGB::Black, CRGB::Red, CRGB::White);

  // Wire together components - weird syntax is a closure
  fadeControl.onFadeOutEnd = ([]() { wakePatterns.next(); });
  fadeControl.onFadeInEnd = ([]() { sleepPatterns.next(); });

  wakePatterns.setState(&wakeState);
  sleepPatterns.setState(&sleepState);
  crossfader.setState(&state);

  // Start components
  fadeControl.setup();
  crossfader.setup();
}

void loop()
{
  // Add entropy to random number generator; we use a lot of it
  random16_add_entropy(random());

  fadeControl.loop();
  crossfader.loop(fadeControl.fade());

  FastLED.show(); // display this frame

  // Run loop() once per frame
  FastLED.delay(1000 / FRAMES_PER_SECOND);
}
