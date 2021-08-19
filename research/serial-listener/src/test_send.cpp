#include <Arduino.h>

void setup() {
  Serial2.begin(9600);
  pinMode(2, INPUT);
}

void loop() {
  if (digitalRead(2) == HIGH) {
  // Send TILT_UP on channel 10
    Serial2.write("\x6a\xca\x6a");
  }
}