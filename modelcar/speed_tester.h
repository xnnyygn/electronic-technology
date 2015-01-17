#include "Arduino.h"

class SpeedTester {
  private:
    int testSide;
    unsigned long lastTestTime;
    int leftCount;
    int rightCount;
    
    float leftSpeed;
    float rightSpeed;
    
  public:
    int init();
    void increaseLeftCount();
    void increaseRightCount();
    int check(unsigned long interval);
    float getLeftSpeed();
    float getRightSpeed();
    
    static const int TEST_SIDE_NONE = 0;
    static const int TEST_SIDE_LEFT = 1;
    static const int TEST_SIDE_RIGHT = 2;
};
