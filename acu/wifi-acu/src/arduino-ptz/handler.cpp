#include "handler.hpp"

namespace ArduinoPTZ {

unsigned long lastCommand = 0;
uint8_t* buffer;
uint8_t* _getSend();
bool firstNullSent = false;

void process(uint8_t val) {
    switch (val) {
        case DIRECTIONS::UP:
            buffer = LL_MCU_PTZ::GENERATE_MOTOR_INSTRUCTION(LL_MCU_PTZ::CHANNEL::CH_10, LL_MCU_PTZ::DIRECTION::UP);
            break;
        case DIRECTIONS::RIGHT:
            buffer = LL_MCU_PTZ::GENERATE_MOTOR_INSTRUCTION(LL_MCU_PTZ::CHANNEL::CH_10, LL_MCU_PTZ::DIRECTION::RIGHT);
            break;
        case DIRECTIONS::DOWN:
            buffer = LL_MCU_PTZ::GENERATE_MOTOR_INSTRUCTION(LL_MCU_PTZ::CHANNEL::CH_10, LL_MCU_PTZ::DIRECTION::DOWN);
            break;
        case DIRECTIONS::LEFT:
            buffer = LL_MCU_PTZ::GENERATE_MOTOR_INSTRUCTION(LL_MCU_PTZ::CHANNEL::CH_10, LL_MCU_PTZ::DIRECTION::LEFT);
            break;
        case DIRECTIONS::STOP:
            buffer = NULL;
    }

    lastCommand = millis();
}

uint8_t* getSend() {
    uint8_t* res = _getSend();
    if (res == NULL) {
        if (firstNullSent) {
            pauseSerial();
        } else {
            firstNullSent = true;
        }
    } else {
        if (firstNullSent) {
            resumeSerial();
            firstNullSent = false;
        }
    }

    return res;
}

uint8_t* _getSend() {
    if (millis() - lastCommand < 500) {
        return buffer;
    }

    return NULL;
}

}  // namespace ArduinoPTZ
