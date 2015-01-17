#include <hcsr04.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include "motor_ctrl.h"
#include "serial_ctrl_server.h"
#include "speed_tester.h"

#define PROTOCOL_MODELCAR_TEST 1
#define PROTOCOL_MODELCAR_BASIC_OP 4
#define PROTOCOL_MODELCAR_SPEED 6
#define PROTOCOL_MODELCAR_FREE 7

#define MCO_ADVANCE 1
#define MCO_STOP 2
#define MCO_BACK 3

#define STATUS_STOP 100
#define STATUS_ADVANCE 101
#define STATUS_BACK 102

#define PIN_BUZZ 11

#define SPEED_TEST_INTERVAL 1000

const float speedK = 3.14 * 6.4 / 4;

// pinM11, pinM12, pinM1Pwm, pinM21, pinM22, pinM2Pwm
MotorCtrl motorCtrl(7, 8, 6, 9, 10, 5);
SerialCtrlServer serialCtrlServer;
HCSR04 hcsr04(4, 12); // pin trig, pin echo
SpeedTester speedTester;

LiquidCrystal_I2C lcd(0x27, 16, 2);

int workingStatus;

void setup() {
  motorCtrl.init();
  serialCtrlServer.init();
  hcsr04.init();
  
  speedTester.init();
  attachInterrupt(0, speedTestIncreaseRightCount, CHANGE);
  
  lcd.init();
  lcd.backlight();
  
  Serial.begin(115200);
  workingStatus = STATUS_STOP;
  
  pinMode(2, INPUT);
  pinMode(3, INPUT);
}

void loop() {
  checkDistance();
  serialCtrlServer.read();
  processCmd();
  speedTest();
}

void checkDistance() {
  float distance = hcsr04.getDistance(2320);
  if(distance >= 0 && distance < 40) {
    if(workingStatus == STATUS_ADVANCE) {
      stopMe();
      buzzWarning();
    }
  }
}

void speedTest() {
  switch(speedTester.check(SPEED_TEST_INTERVAL)) {
    case SpeedTester::TEST_SIDE_LEFT:
      detachInterrupt(0);
      attachInterrupt(1, speedTestIncreaseLeftCount, CHANGE);
      lcd.clear();
      lcd.print("LSV: ");
      lcd.setCursor(0, 1);
      lcd.print(speedTester.getLeftSpeed() * speedK);
      lcd.print("m/s");
      break;
    case SpeedTester::TEST_SIDE_RIGHT:
      attachInterrupt(0, speedTestIncreaseRightCount, CHANGE);
      detachInterrupt(1);
      lcd.clear();
      lcd.print("RSV: ");
      lcd.setCursor(0, 1);
      lcd.print(speedTester.getRightSpeed() * speedK);
      lcd.print("m/s");
      break;
  }
}

void speedTestIncreaseLeftCount() {
  speedTester.increaseLeftCount();
}

void speedTestIncreaseRightCount() {
  speedTester.increaseRightCount();
}

void processCmd() {
  if(serialCtrlServer.cmdAvailable()) {
    switch(serialCtrlServer.getProtocolType()) {
      case PROTOCOL_MODELCAR_TEST:
        serialCtrlServer.done(SerialCtrlServer::RETURN_CODE_OK);
        break;
      case PROTOCOL_MODELCAR_BASIC_OP:
        processModelCarBasicOpCmd();
        serialCtrlServer.done(SerialCtrlServer::RETURN_CODE_OK);
        break;
      case PROTOCOL_MODELCAR_SPEED:
        processModelCarSpeedCmd();
        serialCtrlServer.done(SerialCtrlServer::RETURN_CODE_OK);
        break;
      case PROTOCOL_MODELCAR_FREE:
        processModelCarFreeCmd();
        serialCtrlServer.done(SerialCtrlServer::RETURN_CODE_OK);
        break;
      default:
        serialCtrlServer.done(SerialCtrlServer::RETURN_CODE_UNKNOWN_PROTOCOL);
        break;
    }
  }
}

void processModelCarFreeCmd() {
  int pwm1 = serialCtrlServer.getPayloadByte(0);
  int pwm2 = serialCtrlServer.getPayloadByte(1);
  switch(workingStatus) {
    case STATUS_ADVANCE:
      motorCtrl.run(pwm1, pwm2);
      break;
    case STATUS_BACK:
      motorCtrl.run(-pwm1, -pwm2);
      break;
  }
}

void processModelCarSpeedCmd() {
  int pwm = serialCtrlServer.getPayloadByte(0);
  switch(workingStatus) {
    case STATUS_ADVANCE:
      motorCtrl.run(pwm, pwm);
      break;
    case STATUS_BACK:
      motorCtrl.run(-pwm, -pwm);
      break;
  }
}

void processModelCarBasicOpCmd() {
  switch(serialCtrlServer.getPayloadByte(0)) {
    case MCO_ADVANCE:
      if(workingStatus != STATUS_ADVANCE) {
        motorCtrl.run(180, 180);
        workingStatus = STATUS_ADVANCE;
      }
      break;
    case MCO_STOP:
      stopMe();
      break;
    case MCO_BACK:
      if(workingStatus != STATUS_BACK) {
        motorCtrl.run(-180, -180);
        workingStatus = STATUS_BACK;
      }
      break;
  }
}

void buzzWarning() {
  for(int i = 0; i < 3; i++) {
    delay(250);
    analogWrite(PIN_BUZZ, 128);
    delay(250);
    analogWrite(PIN_BUZZ, LOW);
  }
}

void stopMe() {
  if(workingStatus != STATUS_STOP) {
    motorCtrl.stop();
    workingStatus = STATUS_STOP;
  }
}
