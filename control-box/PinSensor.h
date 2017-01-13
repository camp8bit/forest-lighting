#ifndef InputPin_h
#define InputPin_h

/**
 * A simple input pin
 */
class PinSensor {
  protected:
    byte _pin;
    bool _activeState;
    
  public: 
    PinSensor(byte pin, bool activeState = HIGH) : _pin(pin), _activeState(activeState) {}

    virtual void setup()
    {
      pinMode(_pin, INPUT);
    }
    
    virtual bool isActive()
    {
      return digitalRead(_pin) == _activeState;
    }

};

/**
 * A pullup input pin
 */
class PullupPinSensor : public PinSensor {
  public:
    PullupPinSensor(byte pin, bool activeState = LOW) : PinSensor(pin, activeState) {}

    void setup()
    {
      pinMode(_pin, INPUT_PULLUP);
    }
    
};

#endif
