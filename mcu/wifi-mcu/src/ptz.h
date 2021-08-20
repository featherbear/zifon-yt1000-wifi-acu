#define PT__SERIAL_BAUD 9600

/**
 * RF Channel
 */
enum PT__CHANNEL {
    CH_0,
    CH_1,
    CH_2,
    CH_3,
    CH_4,
    CH_5,
    CH_6,
    CH_7,
    CH_8,
    CH_9,
    CH_10,
    CH_11,
    CH_12,
    CH_13,
    CH_14,
    CH_15,
    CH_16,
    CH_17,
    CH_18,
    CH_19
};

/**
 * Motor Direction
 */
enum PT__DIRECTION {
    UP = 0xCA,    // Tilt Up
    DOWN = 0xC5,  // Tilt down
    LEFT = 0XCC,  // Pan Left
    RIGHT = 0xC3  // Pan Right
};

/**
 * Speed for the Pan / Tilt motor
 */
enum PT__PAN_TILT_SPEED {
    SPEED_1 = 1,
    SPEED_2 = 2,
    SPEED_3 = 3,
    SPEED_4 = 4,
    SPEED_5 = 5,
    SPEED_6 = 6,
    SPEED_7 = 7,
    SPEED_8 = 8,
};

inline uint8_t PT__GENERATE_CHANNEL_BYTE(enum PT__CHANNEL x) { return 0x60 + x; }
inline uint8_t PT__GENERATE_PAN_SPEED_BYTE(enum PT__PAN_TILT_SPEED x) { return 0xA0 + x; }
inline uint8_t PT__GENERATE_TILT_SPEED_BYTE(enum PT__PAN_TILT_SPEED x) { return 0x50 + x; }

/**
 * Generate the 3-byte pan speed packet
 * Note: Make a copy of the returned value immediately! (ceebs allocating memory)
 */
inline uint8_t* PT__GENERATE_PAN_SPEED_INSTRUCTION(enum PT__CHANNEL channel, enum PT__PAN_TILT_SPEED speed) {
    static uint8_t buff[3] = {};
    buff[0] = buff[2] = PT__GENERATE_CHANNEL_BYTE(channel);
    buff[1] = speed;
    return buff;
}

/**
 * Generate the 3-byte tilt speed instruction packet
 * Note: Make a copy of the returned value immediately! (ceebs allocating memory)
 */
inline uint8_t* PT__GENERATE_TILT_SPEED_INSTRUCTION(enum PT__CHANNEL channel, enum PT__PAN_TILT_SPEED speed) {
    static uint8_t buff[3] = {};
    buff[0] = buff[2] = PT__GENERATE_CHANNEL_BYTE(channel);
    buff[1] = speed;
    return buff;
}

/**
 * Generate the 3-byte motor instruction packet
 * Note: Make a copy of the returned value immediately! (ceebs allocating memory)
 */
inline uint8_t* PT__GENERATE_MOTOR_INSTRUCTION(enum PT__CHANNEL channel, enum PT__DIRECTION direction) {
    static uint8_t buff[3] = {};
    buff[0] = buff[2] = PT__GENERATE_CHANNEL_BYTE(channel);
    buff[1] = direction;
    return buff;
}

inline bool PT__IS_PAN_SPEED_BYTE(uint8_t byte) { return !((0xF0 & byte) ^ 0xA0); }
inline bool PT__IS_TILT_SPEED_BYTE(uint8_t byte) { return !((0xF0 & byte) ^ 0x50); }
inline bool PT__IS_DIRECTION_BYTE(uint8_t byte) {
    switch ((enum PT__DIRECTION)byte) {
        case UP:
        case DOWN:
        case LEFT:
        case RIGHT:
            return true;
        default:
            return false;
    }
}

inline bool PT__IS_CHANNEL_BYTE(uint8_t byte) { return 0x60 <= byte && byte <= 0x73; }
