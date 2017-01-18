// Statuses for the PIR sensor
#define PIR_OFF 0
#define PIR_FADEIN 1
#define PIR_ON 2
#define PIR_FADEOUT 3

/**
 * A fader with fade-in, sustain, and fade-out. Extended from the Light Hoops' PIRSensor.
 * 
 * This works well with a PIR motion sensor. Set it for minimum cycle time, so they have
 * a period of about 1-2 seconds. Use this to sustain patterns for the appropriate duration.
 *
 * With the fade in & out, you can build smooth transitions between the "on" and "off" states.
 * 
 * @author Sam Minnee
 */
class ASRFader {
  private:
    PinSensor *_input;
  
    long _lastHigh;
    byte _status;
    unsigned long _lastNow;
    unsigned long _sustain;

    /**
     * Amount of fading, from 0-1
     */
    float _fade;

    /**
     * Fade in / out speeds, in _fade step per ms
     */
    float _fadeInPerMS, _fadeOutPerMS;

  public:
    /**
     * Callback function for the start of a fade-in action
     * Won't trigger unless the status is PIR_OFF prior to the fade-in starting
     */
    void (*onFadeInStart)() = NULL;

    /**
     * Callback function for the end of a fade-in action
     */
    void (*onFadeInEnd)() = NULL;

    /**
     * Callback function for the start of a fade-out action
     */
    void (*onFadeOutStart)() = NULL;

    /**
     * Callback function for the end of a fade-out action
     */
    void (*onFadeOutEnd)() = NULL;

    /**
     * @param input The input pin to use as the sensor
     * @param fadeIn The duration in ms of the fade in
     * @param sustain How long in ms must the sensor be inactive before the fadeOut begins. Includes fadeIn time.
     * @param fadeOut The duration in ms o the fade out
     */
    ASRFader(PinSensor *input, unsigned long fadeIn, unsigned long sustain, unsigned long fadeOut): 
      _lastHigh(0),
      _lastNow(0),
      _status(PIR_OFF),
      _fade(0.0),
      _input(input),
      _sustain(sustain),
      _fadeInPerMS(1.0 / fadeIn), 
      _fadeOutPerMS(1.0 / fadeOut)
    {};

    /**
     * Return the current fade level
     */
    byte fade()
    {
      return (byte)(_fade * 255); 
    }

    void setup()
    {
      _input->setup();
    }

    /**
     * Exeute a loop
     * @param stepSize The number of milliseconds since the last call to loop()
     */
    void loop()
    {
      long now;
      now = millis();

      bool turnOn = false;
      bool turnOff = false;

      // On, just keep track of how recently the signal has been on
      if(_input->isActive()) {
        _lastHigh = now;
        turnOn = true;
      } else if((now - _lastHigh > _sustain)) {
        turnOff = true;
      }

      switch(_status) {
        case PIR_OFF:
          if (turnOn) {
          DEBUG_PRINT("starting fade in");
            _status = PIR_FADEIN;
            if(onFadeInStart != NULL) onFadeInStart();
          }
          break;

        case PIR_FADEOUT:
          if (turnOn) {
            _status = PIR_FADEIN;
          } else {
            _fade -= _fadeOutPerMS * (now - _lastNow);
            if(_fade <= 0.0) {
              DEBUG_PRINT("fulled faded out");
              _fade = 0.0;
              _status = PIR_OFF;
              if(onFadeOutEnd != NULL) onFadeOutEnd();
            }
          }
          break;

        case PIR_ON:
          if (turnOff) {
              DEBUG_PRINT("staring fade out");
            _status = PIR_FADEOUT;
            if(onFadeOutStart != NULL) onFadeOutStart();
          }
          break;

        case PIR_FADEIN:
          if (turnOff) {
            _status = PIR_FADEOUT;
          } else {
            _fade += _fadeInPerMS * (now - _lastNow);
            if(_fade >= 1.0) {
              DEBUG_PRINT("fully faded in");
              _fade = 1.0;
              _status = PIR_ON;
              if(onFadeInEnd != NULL) onFadeInEnd();
            }
          }
          break;
      }
      
      _lastNow = now;
    }
}; 

