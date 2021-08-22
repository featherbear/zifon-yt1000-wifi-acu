#include "serial.hpp"

uint8_t TX_PIN;
uint8_t FN_PORT;
HardwareSerial* SERIAL_INSTANCE;

void initSerial(HardwareSerial& serialInstance, uint8_t txPin, uint8_t function = 0) {
    SERIAL_INSTANCE = &serialInstance;
    TX_PIN = txPin;
    FN_PORT = function;
    SERIAL_INSTANCE->begin(PT__SERIAL_BAUD);
}

void endSerial() {
    SERIAL_INSTANCE->end();
}

// TODO: See if the ESP8266 needs to call the interrupt detach

void pauseSerial() {
#if defined(ESP32)
    // esp32-hal-uart::uartDetachTx
    pinMatrixOutDetach(TX_PIN, false, false);
#endif

    pinMode(TX_PIN, INPUT);  // This line has to appear after pinMatrixOutDetach
}

void resumeSerial() {
    pinMode(TX_PIN, OUTPUT);

#if defined(ESP32)
    // esp32-hal-uart::uartAttachTx
    pinMatrixOutAttach(TX_PIN, FN_PORT, false, false);
#endif
}