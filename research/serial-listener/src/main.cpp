#include <Arduino.h>
#include "serial_listener.h"

void setup() {
  setup_byte_dump();
}

void loop() {
  loop_byte_dump();
}