#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

// TMP sensor connect to A0
#define PIN_TMP 0

int analogValue = 0;
float voltage = 0;
float celsius = 0;
int serialCmd;

LiquidCrystal_I2C lcd(0x27,16,2); 

void setup() {
  Serial.begin(9600);
  
  lcd.init();
  // lcd.backlight();
  lcd.print("ALL CLEAR!");
}

void loop() {
  analogValue = analogRead(PIN_TMP);
  voltage = analogValue * 5000.0 / 1024;
  celsius = (voltage - 500) / 10;
  
  // show temperature on LCD
  lcd.clear();
  lcd.print(celsius);
  
  // send to serial if necessary
  if(Serial.available() && Serial.read() == 71 /* G */) {
    Serial.println(celsius);
  }
  
  delay(1000);
}
