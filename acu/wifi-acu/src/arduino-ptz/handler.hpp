#include "arduino-ptz.hpp"

#ifndef _ARDUINO_PTZ_HANDLER__H
#define _ARDUINO_PTZ_HANDLER__H

namespace ArduinoPTZ {
enum DIRECTIONS {
    UP,
    RIGHT,
    DOWN,
    LEFT,
    STOP = 0xFF
};

void process(uint8_t val);
uint8_t* getSend();
}

#endif