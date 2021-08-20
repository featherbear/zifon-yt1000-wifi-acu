#include "serial.hpp"

uint8_t TX_PIN;
uint8_t FN_PORT;
HardwareSerial* SERIAL_INSTANCE;

void initSerial(HardwareSerial& serialInstance, uint8_t txPin, uint8_t function) {
    TX_PIN = txPin;
    FN_PORT = function;
    SERIAL_INSTANCE = &serialInstance;
    SERIAL_INSTANCE->begin(PT__SERIAL_BAUD);
}

void endSerial() {
    SERIAL_INSTANCE->end();
}

void pauseSerial() {
    // esp32-hal-uart::uartDetachTx
    pinMatrixOutDetach(TX_PIN, false, false);
    pinMode(TX_PIN, INPUT);  // This line has to appear after pinMatrixOutDetach
}

void resumeSerial() {
    // esp32-hal-uart::uartAttachTx
    pinMode(TX_PIN, OUTPUT);
    pinMatrixOutAttach(TX_PIN, FN_PORT, false, false);
}