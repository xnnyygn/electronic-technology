#include "Arduino.h"
#include "speed_tester.h"

int SpeedTester::init() {
  testSide = TEST_SIDE_LEFT;
  leftCount = 0;
  rightCount = 0;
  leftSpeed = 0;
  rightSpeed = 0;
  lastTestTime = millis();
  return testSide;
}

void SpeedTester::increaseLeftCount() {
  leftCount++;
}

void SpeedTester::increaseRightCount() {
  rightCount++;
}

int SpeedTester::check(unsigned long interval) {
  unsigned long now = millis();
  unsigned long deltaTime = now - lastTestTime;
  if(deltaTime > interval) {
    lastTestTime = now;
    
    if(testSide == TEST_SIDE_LEFT) {
      leftSpeed = leftCount * 1.0 / deltaTime;
      rightCount = 0;
      testSide = TEST_SIDE_RIGHT;
    } else {
      rightSpeed = rightCount * 1.0 / deltaTime;
      leftCount = 0;
      testSide = TEST_SIDE_LEFT;
    }
    return testSide;
  }
  return TEST_SIDE_NONE;
}

float SpeedTester::getLeftSpeed() {
  return leftSpeed;
}

float SpeedTester::getRightSpeed() {
  return rightSpeed;
}
