#include <Arduino.h>

void setup() {
    pinMode(7, OUTPUT);
    pinMode(8, INPUT);
}

void loop() {
  //buuzer will buzz when button is pressed
  if (digitalRead(8) == HIGH) {
    digitalWrite(7, HIGH);
  } else {
    digitalWrite(7, LOW);
  }

}
