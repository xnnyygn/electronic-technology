#include "Arduino.h"

class MotorCtrl {
  
  private:
    int mPinM11;
    int mPinM12;
    int mPinM1Pwm;
    
    int mPinM21;
    int mPinM22;
    int mPinM2Pwm;
    
    void setPinValueByPwm(int pinMx1, int pinMx2, int pinMxPwm, int pwm);
    void setPinValue(int pinMx1, int highOrLow1, int pinMx2, int highOrLow2, int pinMxSpeed, int pwm);
    
  public:
    MotorCtrl(int pinM11, int pinM12, int pinM1Speed, int pinM21, int pinM22, int pinM2Speed);
    void init();
    void advance();
    void stop();
    void back();
    void run(int pwm1, int pwm2);
    
};
