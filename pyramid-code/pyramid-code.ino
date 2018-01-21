#define FASTLED_REPEAT_LEDS 4

#include "FastLED.h"

FASTLED_USING_NAMESPACE

// FastLED "100-lines-of-code" demo reel, showing just a few 
// of the kinds of animation patterns you can quickly and easily 
// compose using FastLED.  
//
// This example also shows one easy way to define multiple 
// animations patterns and have them automatically rotate.
//
// -Mark Kriegsman, December 2014

#if defined(FASTLED_VERSION) && (FASTLED_VERSION < 3001000)
#warning "Requires FastLED 3.1 or later; check github for latest code."
#endif

#define DATA_PIN    21
//#define CLK_PIN   4
#define LED_TYPE    WS2811
#define COLOR_ORDER GRB
#define NUM_LEDS    1440
CRGB leds[NUM_LEDS];

#define BRIGHTNESS          128
#define FRAMES_PER_SECOND  50

#define P_REV 24
#define P_OUT 48

// Patterns:
    // 0-23 = forward build out
    // 24 + (0-23) = reverse orser
    // 48 + (0-23) = build from centre
    
byte buildOutPattern[][10] = {
    { 3,   1+P_REV, 2, 4+P_REV },
    { 3,   0+P_OUT, 3+P_OUT, 5+P_OUT },
    { 9,   12+P_REV, 13, 17+P_REV, 9, 6, 8+P_REV, 19+P_REV, 20, 22+P_REV },
    { 9,   7+P_OUT, 10+P_OUT, 11+P_OUT, 14+P_OUT, 15+P_OUT, 16+P_OUT, 18+P_OUT, 21+P_OUT, 23+P_OUT }
};

// At least one function needs to be declared before the pattern lists
void noop() {
}


// List of patterns to cycle through.  Each is defined as a separate function below.
typedef void (*SimplePatternList[])();
SimplePatternList gPatterns = {
  confetti,
  patternPulse, buildingRainbow,
  strobe
};
SimplePatternList gPatternStarts = {
  noop,
  noop, noop,
  noop
};


// Patterns come in 3 intensities, used by the state machine
uint8_t gPatternIntensity = 0; // intensity level of the patterns
uint8_t gPatternIntensities[][2] = {
  { 0, 1 }, // start, count in gPatterns
  { 1, 2 },
  { 3, 1 }
};

uint8_t gCurrentPatternNumber = 0; // Index number of which pattern is current
uint8_t gHue = 0; // rotating "base color" used by many of the patterns

// indicator of steps - increments 1 per beat
unsigned int beatCount = 0;
// 0-60 indicator of beat position
byte beatPos = 0;

// Time the next pattern switch
long gNextSwitchTime = 0;


////////////////////////////////////////////
// MAIN LOOP

void setup() {
  // tell FastLED about the LED strip configuration
  FastLED.addLeds<NEOPIXEL,DATA_PIN>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  
  fill_solid(leds, NUM_LEDS, CRGB::Black);
 
  // set master brightness control
  FastLED.setBrightness(BRIGHTNESS);

  gPatternStarts[gCurrentPatternNumber]();
}

void loop()
{
  // Call the current pattern function once, updating the 'leds' array
  gPatterns[gCurrentPatternNumber]();

  FastLED.delay(1); // millis is out by a factor of 16. this also calls show
  gHue ++;
  
  beatPos = (millis() % 30) * 2;
  beatCount = (millis() / 30);
  
  if(millis() > gNextSwitchTime) {
    nextPattern();
  }

}

#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))

////////////////////////////////////////////
// PATTERN STATE MACHINE

void nextPattern()
{
  byte diceRoll = random8();
  // Simple state machine of intensity changes
  switch(gPatternIntensity) {
    case 0: // quiet (confetti, etc)
      if(diceRoll < 13) nextPatternOfIntensity(2); // 5% chance of high pattern
      else nextPatternOfIntensity(0); // 95% chance of low pattern
      break;

    case 1: // mid (most patterns)
      if(diceRoll < 26) nextPatternOfIntensity(0); // 10% chance of low pattern
      else if(diceRoll < 51) nextPatternOfIntensity(2); // 10% chance of high pattern
      else nextPatternOfIntensity(1); // 80% chance of mid pattern
      break;

    case 2: // intense (strobe, etc)
      if(diceRoll < 26) nextPatternOfIntensity(2); // 10% chance of high pattern
      else if(diceRoll < 77) nextPatternOfIntensity(0); // 20% chance of low pattern
      else nextPatternOfIntensity(1); // 70% chance of mid pattern
      break;
  }
}

void nextPatternOfIntensity(byte intensity)
{
  long delays[] = {
      180, //3750, // 60s for low intensity
      180, //1250, // 20s for mid intensity
      180, //625, // 10s for high intensity
  };

  byte start = gPatternIntensities[intensity][0];
  byte count = gPatternIntensities[intensity][1];

  byte nextPattern = start + random8(count); 
  while(nextPattern == gCurrentPatternNumber && count > 1) { // ensure a new pattern unless there's only one
    nextPattern = start + random8(count); 
  }

  gCurrentPatternNumber = nextPattern;
  gPatternIntensity = intensity;
  gNextSwitchTime = millis() + delays[intensity];

  gPatternStarts[gCurrentPatternNumber]();
  
}

////////////////////////////////////////////
// PATTERNS

void buildingRainbow() 
{
  fill_solid(leds, NUM_LEDS, CRGB::Black);
  fill_rainbow( leds, (millis() * 16) % NUM_LEDS, gHue, 4);
}

void patternPulse()
{
  fill_solid(leds, NUM_LEDS, CRGB::Black);

  byte patternCount = beatCount % 4;
  byte numSteps = buildOutPattern[patternCount][0];
  for(byte i=1; i<=numSteps; i++) {
    drawStrip(buildOutPattern[patternCount][i], beatPos);
  }
}

void confetti() 
{
  // random colored speckles that blink in and fade smoothly
  fadeToBlackBy( leds, NUM_LEDS, 10);
  for(byte i=0; i<3; i++) {
    leds[random16(NUM_LEDS)] += CHSV( gHue + random8(64), 200, 255);
  }
}

////////////////////////////////////////////
// SUPPORT FUNCTIONS

void drawStrip(byte stripNum, byte amount)
{
  // Regular direction
  if(stripNum < 24) {
    fill_rainbow( leds + (stripNum * 60), amount, gHue + (stripNum * 60), 4);

  // Reverse direction
  } else if(stripNum < 48) {
    stripNum -= 24;
    fill_rainbow( leds + (stripNum * 60) + 60-amount, amount, gHue + (stripNum * 60), 4);
    
  // Buildout direction
  } else if(stripNum < 72) {
    stripNum -= 48;
    fill_rainbow( leds + (stripNum * 60) + (60-amount)/2, amount, gHue + (stripNum * 60), 4);
  }
}

void strobe()
{
  CRGB colour;
  if(gHue % 2 == 0) colour = CRGB::Black; // gHue increments each frame, we use this to alternate frames
  else colour = CRGB::White;

  fill_solid(leds, NUM_LEDS, colour);
}

void sinelon()
{
  // a colored dot sweeping back and forth, with fading trails
  fadeToBlackBy( leds, NUM_LEDS, 20);
  int pos = beatsin16( 13, 0, NUM_LEDS-1 );
  leds[pos] += CHSV( gHue, 255, 192);
}

void bpm()
{
  // colored stripes pulsing at a defined Beats-Per-Minute (BPM)
  uint8_t BeatsPerMinute = 62;
  CRGBPalette16 palette = PartyColors_p;
  uint8_t beat = beatsin8( BeatsPerMinute, 64, 255);
  for( int i = 0; i < NUM_LEDS; i++) { //9948
    leds[i] = ColorFromPalette(palette, gHue+(i*2), beat-gHue+(i*10));
  }
}

void juggle() {
  // eight colored dots, weaving in and out of sync with each other
  fadeToBlackBy( leds, NUM_LEDS, 20);
  byte dothue = 0;
  for( int i = 0; i < 8; i++) {
    leds[beatsin16( i+7, 0, NUM_LEDS-1 )] |= CHSV(dothue, 200, 255);
    dothue += 32;
  }
}


void white()
{
  fill_solid(leds, NUM_LEDS, CRGB::White);
}

