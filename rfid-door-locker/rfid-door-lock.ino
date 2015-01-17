/******************************
 * DOOR LOCK SYSTEM
 * Author: XnnYygn
 * Date: 2014-10-31
 *
 ******************************
 * PIN data sheet
 * ===========
 * RFRC522 PIN
 * 
 * RST    D9
 * SS     D10
 * MOSI   D11
 * MISO   D12
 * SCK    D13
 * ============
 * Servo PIN
 *
 * DATA D6
 * ============
 * LED PIN
 *
 * PASSED(green)   D7
 * FAILED(red)     D8
 * ============
 * BUZZ PIN
 *
 * VCC D3
 * ============
 * LCD I2C PIN
 *
 * SDA A4
 * SCL A5
 * ============
 ******************************
 * Enjoy~
 */

#include <Servo.h>
#include <SPI.h>
#include <MFRC522.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

// SWF stands for STATUS_WAITING_FOR
#define SWF_CARD 0
#define SWF_RESULT 1

#define RFID_SS_PIN 10
#define RFID_RST_PIN 9

#define SERVO_DATA_PIN 6

#define LED_PASSED_PIN 7 // green
#define LED_FAILED_PIN 8 // red

#define BUZZ_PIN 3

////////////////////////////////////////
// variables
////////////////////////////////////////

int workingStatus = SWF_CARD;
int validationResult;

// create MFRC522 controller
MFRC522 mfrc522(RFID_SS_PIN, RFID_RST_PIN);

// servo controller
Servo myServo;

// LCD I2C
LiquidCrystal_I2C lcd(0x27,16,2);

////////////////////////////////////////
// methods
////////////////////////////////////////

void setup() {
  // set serial to 9600, same to raspberry pi pySerial
  Serial.begin(9600);
  
  // initialize MFRC522
  SPI.begin();
  mfrc522.PCD_Init();
  
  // initialize servo
  // myServo.attach(SERVO_DATA_PIN);
  
  // set pin for LEDs
  pinMode(LED_PASSED_PIN, OUTPUT);
  pinMode(LED_FAILED_PIN, OUTPUT);
  
  // set pin for buzz
  pinMode(BUZZ_PIN, OUTPUT);
  
  // initialize the lcd 
  lcd.init();                      
  lcd.print("ALL CLEAR!");
}

void loop() {
  switch(workingStatus) {
    case SWF_CARD: 
      waitingForCard(); 
      break;
    case SWF_RESULT:
      waitingForResult();
      break;
  }
}

void waitingForCard() {
  if (!mfrc522.PICC_IsNewCardPresent()) {
    return;
  }
  if ( ! mfrc522.PICC_ReadCardSerial()) {
    return;
  }
  
  // show card id on LCD
  lcd.backlight();
  lcd.clear();
  
  // send card id to serial and LCD
  Serial.print("CARD_ID: ");
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    if(mfrc522.uid.uidByte[i] < 0x10) {
      Serial.print("0");
      lcd.print("0");
    }
    Serial.print(mfrc522.uid.uidByte[i], HEX);
    lcd.print(mfrc522.uid.uidByte[i], HEX);
  }
  Serial.println();
  
  // change working status
  workingStatus = SWF_RESULT;
  
  mfrc522.PICC_HaltA();
}

void waitingForResult() {
  if(Serial.available()) {
    validationResult = Serial.read();
    Serial.print("VALIDATION RESULT: ");
    Serial.println(validationResult);
    
    // buzz
    analogWrite(BUZZ_PIN, 128);
    delay(500);
    analogWrite(BUZZ_PIN, LOW);
    
    // set LCD cursor to next line
    lcd.setCursor(0, 1);
    
    if(validationResult == 84 /* T */) {
      onValidationPassed();
    } else {
      onValidationFailed();
    }
    
    // delay 1 second before next operation
    delay(1000);
    
    // return to init status
    workingStatus = SWF_CARD;
    lcd.noBacklight();
    lcd.clear();
    lcd.print("ALL CLEAR");
  }
}

void onValidationPassed() {
  lcd.print("PASSED");
  
  // turn on passed LED
  digitalWrite(LED_PASSED_PIN, HIGH);
  
  // turn servo
  myServo.attach(SERVO_DATA_PIN);
  delay(1000);
  myServo.write(180);
  delay(1000);
  myServo.write(0);
  delay(1000);
  myServo.detach();
  
  // turn off passed LED
  digitalWrite(LED_PASSED_PIN, LOW);
}

void onValidationFailed() {
  lcd.print("FAILED");
  
  // turn on failed LED
  digitalWrite(LED_FAILED_PIN, HIGH);
  delay(1000);
  
  // turn off failed LED
  digitalWrite(LED_FAILED_PIN, LOW);
}
