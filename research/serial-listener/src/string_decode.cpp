#include <Arduino.h>
#include "serial_listener.h"

void setup_string_decode() {
  Serial.begin(9600);
  Serial2.begin(9600);
}

void loop_string_decode() {
  if (Serial2.available()) {
    String s = Serial2.readStringUntil('\n');
    Serial.println(s);
    for (int i = 0; i < s.length(); i++) {
      Serial.printf("%p ", s[i]);
    }
  }
}