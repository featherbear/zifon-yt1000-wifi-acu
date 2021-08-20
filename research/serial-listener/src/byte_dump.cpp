#include <Arduino.h>
#include "serial_listener.h"

void setup_byte_dump() {
  Serial.begin(9600);
  Serial2.begin(9600);
}

void loop_byte_dump() {
  Serial.print(Serial2.read(), HEX);
}