#include "handler.hpp"

namespace ArduinoPTZ {

void process(DIRECTIONS direction) {
    switch (direction) {
        case DIRECTIONS::UP:
            SERIAL_INSTANCE->write(LL_MCU_PTZ::GENERATE_MOTOR_INSTRUCTION(LL_MCU_PTZ::CHANNEL::CH_10, LL_MCU_PTZ::DIRECTION::UP), 3);
            break;
        case DIRECTIONS::RIGHT:
            SERIAL_INSTANCE->write(LL_MCU_PTZ::GENERATE_MOTOR_INSTRUCTION(LL_MCU_PTZ::CHANNEL::CH_10, LL_MCU_PTZ::DIRECTION::RIGHT), 3);
            break;
        case DIRECTIONS::DOWN:
            SERIAL_INSTANCE->write(LL_MCU_PTZ::GENERATE_MOTOR_INSTRUCTION(LL_MCU_PTZ::CHANNEL::CH_10, LL_MCU_PTZ::DIRECTION::DOWN), 3);
            break;
        case DIRECTIONS::LEFT:
            SERIAL_INSTANCE->write(LL_MCU_PTZ::GENERATE_MOTOR_INSTRUCTION(LL_MCU_PTZ::CHANNEL::CH_10, LL_MCU_PTZ::DIRECTION::LEFT), 3);
            break;
    }
}
}  // namespace ArduinoPTZ
