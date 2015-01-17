#include <LiquidCrystal_I2C.h>
#include <Wire.h>

#define PIN_RELAY 4
#define PIN_PWM_LED 3

#define STATUS_WAIT_LEN 0
#define STATUS_WAIT_PT 1
#define STATUS_WAIT_PL 2
#define STATUS_DONE 3

#define PT_TEST 1
#define PT_SIMPLE_SWITCH 2
#define PT_SIMPLE_PWM 3

LiquidCrystal_I2C lcd(0x27, 16, 2);

int dataLength;
int protocolType;
int payload;

int workingStatus;
int cursorPos = 0;

void setup() {
  pinMode(PIN_RELAY, OUTPUT);
  pinMode(PIN_PWM_LED, OUTPUT);
  
  Serial.begin(115200);
  workingStatus = STATUS_WAIT_LEN;
  
  lcd.init();
  lcd.backlight();
}

void loop() {
  if(workingStatus == STATUS_DONE) {
    switch(protocolType){
      case PT_TEST: 
        replyCode(48);
        break;
      case PT_SIMPLE_SWITCH:
        digitalWrite(PIN_RELAY, payload == 1 ? HIGH : LOW);
        replyCode(48);
        break;
      case PT_SIMPLE_PWM:
        if(payload < 0) payload = 0;
        if(payload > 255) payload = 255;
        analogWrite(PIN_PWM_LED, payload);
        replyCode(48);
        break;
    }
    workingStatus = STATUS_WAIT_LEN;
  }
  if(Serial.available()) {
    if(workingStatus == STATUS_WAIT_LEN) {
      dataLength = Serial.read();
      lcd.clear();
      lcd.print(dataLength);
      lcd.print(' ');
      workingStatus = STATUS_WAIT_PT;
    } else if(workingStatus == STATUS_WAIT_PT) {
      protocolType = Serial.read();
      lcd.print(protocolType);
      lcd.print(' ');
      if(dataLength > 1) {
        workingStatus = STATUS_WAIT_PL;
      } else {
        workingStatus = STATUS_DONE;
      }
    } else if(workingStatus == STATUS_WAIT_PL) {
      payload = Serial.read();
      lcd.print(payload);
      workingStatus = STATUS_DONE;
    }
  }
}

void replyCode(byte code) {
  Serial.write(1);
  Serial.write(code);
}
