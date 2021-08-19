#include <Arduino.h>

void setup() {
  Serial.begin(9600);
  Serial2.begin(9600);
}

void loop() {
  Serial.print(Serial2.read(), HEX);
}