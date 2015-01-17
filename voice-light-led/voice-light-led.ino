#define PIN_VOICE 2
#define PIN_LIGHT 0
#define PIN_LED 13

void setup() {
  pinMode(PIN_VOICE, INPUT);
  pinMode(PIN_LED, OUTPUT);
}

void loop() {
  if(digitalRead(PIN_VOICE) == LOW && analogRead(PIN_LIGHT) > 200){
    digitalWrite(PIN_LED, HIGH);
    delay(200);
    digitalWrite(PIN_LED, LOW);
    delay(10);
  }
}
