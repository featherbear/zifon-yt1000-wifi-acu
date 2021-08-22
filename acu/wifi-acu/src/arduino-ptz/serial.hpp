#include "arduino-ptz.hpp"

void initSerial(HardwareSerial& serialInstance, uint8_t txPin, uint8_t function);
void endSerial();
void pauseSerial();
void resumeSerial();