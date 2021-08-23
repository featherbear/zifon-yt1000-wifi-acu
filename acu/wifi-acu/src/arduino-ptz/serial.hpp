#include "arduino-ptz.hpp"

#ifndef _ARDUINO_PTZ_SERIAL__H
#define _ARDUINO_PTZ_SERIAL__H

namespace ArduinoPTZ {
extern HardwareSerial* SERIAL_INSTANCE;
void initSerial(HardwareSerial& serialInstance, uint8_t txPin);
void initSerial(HardwareSerial& serialInstance, uint8_t txPin, uint8_t function);
void endSerial();
void pauseSerial();
void resumeSerial();
}  // namespace ArduinoPTZ

#endif