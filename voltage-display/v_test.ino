#define PIN_DATA 6
#define PIN_LATCH 8
#define PIN_CLOCK 10

#define PIN_D_2 4
#define PIN_D_3 3
#define PIN_D_4 2

byte digits[] = {252, 96, 218, 242, 102, 182, 190, 224, 246, 238};

void setup() {
  Serial.begin(9600);
  
  pinMode(PIN_DATA, OUTPUT);
  pinMode(PIN_LATCH, OUTPUT);
  pinMode(PIN_CLOCK, OUTPUT);
  
  pinMode(PIN_D_2, OUTPUT);
  digitalWrite(PIN_D_2, HIGH);
  
  pinMode(PIN_D_3, OUTPUT);
  digitalWrite(PIN_D_3, HIGH);
  
  pinMode(PIN_D_4, OUTPUT);
  digitalWrite(PIN_D_4, HIGH);
}

void loop() {
  // read voltage
  int analogValue = analogRead(0);
  float voltage = 0.0048 * analogValue;
  byte numbers = voltage * 100;
  
  for(byte i = 0; i < 20; i++) {
    displayNumber(numbers / 100, true, PIN_D_2);
    displayNumber((numbers % 100) / 10, false, PIN_D_3);
    displayNumber(numbers % 10, false, PIN_D_4);
  }
}

void displayNumber(byte n, bool withPoint, byte pin) {
  digitalWrite(pin, LOW);
  digitalWrite(PIN_LATCH, LOW);
  shiftOut(PIN_DATA, PIN_CLOCK, LSBFIRST, digits[n] + (withPoint ? 1 : 0));
  digitalWrite(PIN_LATCH, HIGH);
  delay(10);
  digitalWrite(pin, HIGH);
}
