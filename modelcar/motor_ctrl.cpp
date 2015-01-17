#include "Arduino.h"
#include "motor_ctrl.h"

MotorCtrl::MotorCtrl(int pinM11, int pinM12, int pinM1Pwm, int pinM21, int pinM22, int pinM2Pwm) {
  mPinM11 = pinM11;
  mPinM12 = pinM12;
  mPinM1Pwm = pinM1Pwm;
  
  mPinM21 = pinM21;
  mPinM22 = pinM22;
  mPinM2Pwm = pinM2Pwm;
}

void MotorCtrl::init() {
  pinMode(mPinM11, OUTPUT);
  pinMode(mPinM12, OUTPUT);
  pinMode(mPinM1Pwm, OUTPUT);
  
  pinMode(mPinM21, OUTPUT);
  pinMode(mPinM22, OUTPUT);
  pinMode(mPinM2Pwm, OUTPUT);
}

void MotorCtrl::advance() {
  run(255, 255);
}

void MotorCtrl::stop() {
  run(0, 0);
}

void MotorCtrl::back() {
  run(-255, -255);
}

void MotorCtrl::run(int pwm1, int pwm2) {
  setPinValueByPwm(mPinM11, mPinM12, mPinM1Pwm, pwm1);
  setPinValueByPwm(mPinM21, mPinM22, mPinM2Pwm, pwm2);
}

void MotorCtrl::setPinValueByPwm(int pinMx1, int pinMx2, int pinMxPwm, int pwm) {
  if(pwm > 255) {
    setPinValue(pinMx1, HIGH, pinMx2, LOW, pinMxPwm, 255);
  } else if(pwm > 0) {
    setPinValue(pinMx1, HIGH, pinMx2, LOW, pinMxPwm, pwm);
  } else if(pwm == 0) {
    setPinValue(pinMx1, LOW, pinMx2, LOW, pinMxPwm, 0);
  } else if(pwm > -255) {
    setPinValue(pinMx1, LOW, pinMx2, HIGH, pinMxPwm, -pwm);
  } else {
    setPinValue(pinMx1, LOW, pinMx2, HIGH, pinMxPwm, 255);
  }
}

void MotorCtrl::setPinValue(int pinMx1, int highOrLow1, int pinMx2, int highOrLow2, int pinMxPwm, int pwm) {
  digitalWrite(pinMx1, highOrLow1);
  digitalWrite(pinMx2, highOrLow2);
  analogWrite(pinMxPwm, pwm);
}
