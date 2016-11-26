#include <FastLED.h>

#define LED_PIN     3
#define COLOR_ORDER GRB
#define CHIPSET     WS2812B
#define NUM_LEDS    90
#define PIR_PIN     5

#define BRIGHTNESS  255
#define FRAMES_PER_SECOND 120

bool gReverseDirection = false;

CRGB leds[NUM_LEDS];

CRGBPalette16 gPal;

// Last time sensor was triggered
unsigned long lastTriggered;

// First trigger of sequence
unsigned long firstTriggered;

void setup() {
  Serial.begin(9600);

  delay(500); // sanity delay
  FastLED.addLeds<CHIPSET, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.setBrightness( BRIGHTNESS );

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
  // gPal = CRGBPalette16( CRGB::Black, CRGB::FireBrick, CRGB::DeepPink,  CRGB::White);
  gPal = CRGBPalette16( CRGB::Black, CRGB(0,0,255), CRGB::White);

  // gPal = CRGBPalette16( CRGB::Black, CRGB::Red, CRGB::Pink,  CRGB::White);

  // Third, here's a simpler, three-step gradient, from black to red to white
  //   gPal = CRGBPalette16( CRGB::Black, CRGB::Red, CRGB::White);

  SetupExplosions();
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
  // This section handles fading the animation in and out depending on when the
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
  } else {
    fade = 0;
  }

  byte bF = max(0, min(255, fade));

  // DrawNoise(bF);
  // DrawPlasmaDirectional(bF);
  DrawPlasmaTwo(bF);
  // DrawPlasma(bF);
  // DrawExplosions(bF);
  // Fire2012WithPalette(bF); // run simulation frame, using palette colors

  FastLED.show(); // display this frame
  FastLED.delay(1000 / FRAMES_PER_SECOND);
}

/* Noise */
void DrawNoise (byte fade) {
  for (int i = 0; i < NUM_LEDS; i++) {
    byte c = random(3) == 0 ? random(255) : CRGB::Black;
    byte colorindex = scale8( c, 200);
    CRGB color = ColorFromPalette( gPal, colorindex);
    leds[i] = color;
    leds[i] %= fade;
  }
}

/* Draw a plasma that moves up and down*/
void DrawPlasmaDirectional (byte fade) {
  for (int i = 0; i < NUM_LEDS; i++) {
    long t = sin8((long) millis() / 17) * 2;
    t += sin8((long) millis() / 11) * 2;
    byte c = sin8((long) i * 22 + t);
    byte colorindex = scale8( c, 200);
    CRGB color = ColorFromPalette( gPal, colorindex);
    leds[i] = color;
    leds[i] %= fade;
  }
}


/* Draw a two factor plasma */
void DrawPlasmaTwo (byte fade) {
  for (int i = 0; i < NUM_LEDS; i++) {
    byte c = sin8((long) i * 31 - millis() / 3);
    byte b = sin8((long) i * 23 - millis() / 5);
    byte colorindex = scale8((b / 2 + c / 2), 200);
    CRGB color = ColorFromPalette( gPal, colorindex);
    leds[i] = color;
    leds[i] %= fade;
  }
}

/* Draw a sin wave (single factor plasma) that moves up the strip */
void DrawPlasma (byte fade) {
  for (int i = 0; i < NUM_LEDS; i++) {
    byte c = sin8((long) i * 30 - millis() / 2);
    byte colorindex = scale8( c, 200);
    CRGB color = ColorFromPalette( gPal, colorindex);
    leds[i] = color;
    leds[i] %= fade;
  }
}


/* Draws explosions that extend out from a start point and then fade out */

struct explosion {
  bool active;
  int position;
  int size;
  CRGB color;
};

#define NUM_EXPLOSIONS 10
explosion explosions[NUM_EXPLOSIONS];

void SetupExplosions()
{
  for (int i=0; i < NUM_EXPLOSIONS; i++){
    explosions[i].active = false;
  }
}

#define SPAWN_EXPLOSION 5
#define EXPLOSION_SIZE 30

void DrawExplosions(byte fade)
{
  int i;
  int x;
  
  if (random(100) < SPAWN_EXPLOSION) {
    int i = random(NUM_EXPLOSIONS - 1);
    explosions[i].active = true;
    explosions[i].size = 0;
    explosions[i].position = random(NUM_LEDS);
    explosions[i].color = (random(2) < 1) ? CRGB::Green : CRGB::Aqua;
  }

  for (i = 0; i < NUM_LEDS; i++) {
    leds[i].fadeToBlackBy(16);
  }
  
  for (i=0; i < NUM_EXPLOSIONS; i++){
    if (!explosions[i].active) {
      continue;
    }

    explosions[i].size++;

    if (explosions[i].size > EXPLOSION_SIZE) {
      explosions[i].active = false;
      continue;
    }
    
    for (x = -explosions[i].size / 2; x < explosions[i].size / 2; x++) {
      int y = explosions[i].position + x;
      
      if (y < 0 || y >= NUM_LEDS) {
        continue;
      }

      // use 240 instead of 255 to only use part of the palette
      byte modulus = (int) 240 - (240 * abs(x) / (explosions[i].size / 2));
      CRGB color = ColorFromPalette(gPal, modulus);
      leds[y] = color; // explosions[i].color % modulus;
      leds[x] %= fade;
    }
  }
}

#define COOLING  80
#define SPARKING 120

void Fire2012WithPalette(byte fade)
{
  // Array of temperature readings at each simulation cell
  static byte heat[NUM_LEDS];

  bool pirOn = digitalRead(PIR_PIN) == HIGH;

  // Step 1.  Cool down every cell a little
  for ( int i = 0; i < NUM_LEDS; i++) {
    heat[i] = qsub8( heat[i],  random8(0, ((COOLING * 10) / NUM_LEDS) + 2));
  }

  // Step 2.  Heat from each cell drifts 'up' and diffuses a little
  for ( int k = NUM_LEDS - 1; k >= 2; k--) {
    heat[k] = (heat[k - 1] + heat[k - 2] + heat[k - 2] ) / 3;
  }

  // Step 3.  Randomly ignite new 'sparks' of heat near the bottom
  if ( random8() < SPARKING ) {
    int y = random8(7);
    heat[y] = qadd8( heat[y], random8(160, 255) );
  }

  // Step 4.  Map from heat cells to LED colors
  for ( int j = 0; j < NUM_LEDS; j++) {
    // Scale the heat value from 0-255 down to 0-240
    // for best results with color palettes.
    byte colorindex = scale8( heat[j], 240);
    CRGB color = ColorFromPalette( gPal, colorindex);
    int pixelnumber;
    if ( gReverseDirection ) {
      pixelnumber = (NUM_LEDS - 1) - j;
    } else {
      pixelnumber = j;
    }
    leds[pixelnumber] = color;
    leds[pixelnumber] %= fade;
  }
}

