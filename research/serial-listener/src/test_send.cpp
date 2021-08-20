#include <Arduino.h>
#include "serial_listener.h"

void setup_send() {
  Serial2.begin(9600);
  pinMode(2, INPUT);
}

void loop_send() {
  if (digitalRead(2) == HIGH) {
  // Send TILT_UP on channel 10
    Serial2.write("\x6a\xca\x6a");
  }
}