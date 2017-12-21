#include <FastLED.h>
#include <avr/wdt.h>

//#define DEV_MODE
#define DEBUG

#define LED_PIN     3
#define COLOR_ORDER GRB
#define CHIPSET     WS2812B

// Number of milliseconds to stay on for
#define DURATION (3600l * 1000l * 12l)
#define RESET_DURATION (3600l * 1000l * 24l)

#define BRIGHTNESS  255
#define FRAMES_PER_SECOND 120
#define STRIPE_WIDTH 30

#define BREATHS_PER_MINUTE 15
#define HEARTBEAT 70

// ms of fade up
#define FADE_UP 5000
#define MIN_PERIOD (FADE_UP + 5000)
#define FADE_DOWN 5000

// Time in seconds between palette cycles
#define PALETTE_CYCLE_TIME (60l * 60l)

#ifdef DEV_MODE
  #define NUM_LEDS    90
  #define PIR_PIN     (new PullupPinSensor(6))

#else
  #define NUM_LEDS    90
  #define PIR_PIN     (new PinSensor(5))
#endif

#ifdef DEBUG
  #define DEBUG_PRINT(msg) (Serial.println(msg))
#else
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
#include "ZigZag.h"

// Sleep code
#include <avr/interrupt.h>
#include <avr/sleep.h>

// Fader control will fade the lights up when someone enters the scene, then keep
// them faded up for a minimum period. If the PIR triggers again in this time
// we keep the lights up. The goal is to have smooth transitions when someone
// walks up, and if people stand around under the lamp posts (and trigger the
// PIR every few seconds), the lamp posts will stay  lit until the person
// walks away for good.
ASRFader fadeControl(PIR_PIN, FADE_UP, MIN_PERIOD, FADE_DOWN);

// Configure palette
CRGBPalette16 palette(CRGB::Black, CRGB::Red, CRGB::Yellow, CRGB::White);
    // Other options:
    // CRGBPalette16 palette( CRGB::Black, CRGB::Red, CRGB::Yellow, CRGB::White);
    // CRGBPalette16 palette( CRGB::Black, CRGB::Blue, CRGB::Aqua,  CRGB::White);
    // CRGBPalette16 palette( CRGB::Black, CRGB(0,0,255), CRGB::White);
    // CRGBPalette16 palette( CRGB::Black, CRGB::ForestGreen, CRGB::Yellow,  CRGB::White);
    // CRGBPalette16 palette( CRGB::Black, CRGB::Red, CRGB::Pink,  CRGB::White);
    // CRGBPalette16 palette( CRGB::Black, CRGB::Red, CRGB::White);

// Configure the patterns available
Pattern *wakePatternItems[] = { new PlasmaTwo(), new ZigZag(), new Explosions(), new Fire2012(), new PlasmaDirectional() };
PatternList wakePatterns(5, wakePatternItems);

Pattern *sleepPatternItems[] = { new Twinkle() };
PatternList sleepPatterns(1, sleepPatternItems);

// Create a crossfader for blending between sleep and wake
CrossFader crossfader(&sleepPatterns, &wakePatterns);

PatternState state, sleepState, wakeState;

byte palIdx = 0;

void (* resetArduino) (void) = 0;

void setup() {
  Serial.begin(9600);
  Serial.println("control-box 2017 - by camp8bit");
  Serial.print("compiled on ");
  Serial.println(__DATE__);

  delay(500); // sanity delay
  FastLED.addLeds<CHIPSET, LED_PIN, COLOR_ORDER>(state.leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.setBrightness( BRIGHTNESS );

  FastLED.setMaxPowerInVoltsAndMilliamps(5, 500);

  // Wire together components - weird syntax is a closure
  fadeControl.onFadeOutEnd = ([]() { wakePatterns.next(); });
  fadeControl.onFadeInEnd = ([]() { sleepPatterns.next(); });

  sleepState.setPalette(&palette);
  wakeState.setPalette(&palette);

  wakePatterns.setState(&wakeState);
  sleepPatterns.setState(&sleepState);
  crossfader.setState(&state);

  random16_add_entropy(random());
  wakePatterns.rand();

  // Start components
  fadeControl.setup();
  crossfader.setup();
}

long readVcc() {
  // Read 1.1V reference against AVcc
  // set the reference to Vcc and the measurement to the internal 1.1V reference
  #if defined(__AVR_ATmega32U4__) || defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
    ADMUX = _BV(REFS0) | _BV(MUX4) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
  #elif defined (__AVR_ATtiny24__) || defined(__AVR_ATtiny44__) || defined(__AVR_ATtiny84__)
    ADMUX = _BV(MUX5) | _BV(MUX0);
  #elif defined (__AVR_ATtiny25__) || defined(__AVR_ATtiny45__) || defined(__AVR_ATtiny85__)
    ADMUX = _BV(MUX3) | _BV(MUX2);
  #else
    ADMUX = _BV(REFS0) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
  #endif  

  delay(2); // Wait for Vref to settle
  ADCSRA |= _BV(ADSC); // Start conversion
  while (bit_is_set(ADCSRA,ADSC)); // measuring

  uint8_t low  = ADCL; // must read ADCL first - it then locks ADCH  
  uint8_t high = ADCH; // unlocks both

  long result = (high<<8) | low;

  result = 1125300L / result; // Calculate Vcc (in mV); 1125300 = 1.1*1023*1000
  return result; // Vcc in millivolts
}

#define UNDER_VOLT 4000
#define UNDER_VOLT_SAMPLES 60

void powerOff() {
    CRGB *leds = crossfader.getLEDs();

    fill_solid(leds, NUM_LEDS, CRGB::Black);
    leds[0] = CRGB(128, 0, 0);
    FastLED.show(); // display this frame
    
    set_sleep_mode(SLEEP_MODE_PWR_DOWN);
    sleep_enable();              
    sleep_mode();                
}

byte failCount = 0;

void loop()
{
  if (readVcc() < UNDER_VOLT) {
    failCount++;
  } else {
    failCount = 0;
  }

  // If undervolt 60 times in a row, power off
  if (failCount >= UNDER_VOLT_SAMPLES) {
    powerOff();
    return;
  }
  
  // Add entropy to random number generator; we use a lot of it
  random16_add_entropy(random());

  CRGB *leds = crossfader.getLEDs();

  // Run patterns
  if (millis() <= DURATION) {
    fadeControl.loop();
    crossfader.loop(fadeControl.fade());
  
    // Display the progress of the night
    long i = NUM_LEDS * millis() / DURATION;
  
    // Ghetto clamp
    i = min(max(i, 1), NUM_LEDS - 2);
    
    leds[i-1] = CRGB::Black;
    leds[i] = CRGB::White;
    leds[i+1] = CRGB::Black;
  
  // Turn off
  } else {
    fill_solid(leds, NUM_LEDS, CRGB::Black);

    // Blink led - 0.2s every 5s
    if (millis() % 5000 < 200) {
      leds[0] = CRGB(0, 0, 128);
    }
  }

  // Restart the arduino
  if (millis() > RESET_DURATION) {
    resetArduino();
  }

  // Change palette periodically
  EVERY_N_SECONDS(PALETTE_CYCLE_TIME) {
    palIdx = (palIdx + 1) % 9;
    switch(palIdx) {
      case 0: palette = CRGBPalette16(CRGB::Black, CRGB::Red, CRGB::Yellow, CRGB::White); break;
      case 1: palette = CRGBPalette16(CRGB::Black, CRGB::MediumPurple, CRGB::DeepPink, CRGB::White); break;
      case 2: palette = CRGBPalette16( CRGB::Black, CRGB::Blue, CRGB::Aqua,  CRGB::White); break;
      case 3: palette = RainbowColors_p; palette[0] = CRGB::Black; break;
      case 4: palette = CRGBPalette16(CRGB::Black, CRGB::White, CRGB::Yellow, CRGB::Black); break;
      case 5: palette = CRGBPalette16( CRGB::Black, CRGB::ForestGreen, CRGB::Lime,  CRGB::White); break;
      case 6: palette = CRGBPalette16(CRGB::Black, CRGB(16,16,16), CRGB(64,64,64), CRGB::Green); break;
      case 7: palette = CRGBPalette16(CRGB::Black, CRGB::White, CRGB::Black, CRGB::White); break;
      case 8: palette = CRGBPalette16(CRGB::Black, CRGB::Red, CRGB::Yellow, CRGB::White); break;
    }
    
  }
  
  FastLED.show(); // display this frame

  // Run loop() once per frame
  FastLED.delay(1000 / FRAMES_PER_SECOND);
}
