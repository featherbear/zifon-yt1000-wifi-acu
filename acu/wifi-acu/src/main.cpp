#include <Arduino.h>

#include "arduino-ptz/arduino-ptz.hpp"

#define BOOT_CONFIG_PIN D5

#define UART_SERIAL_DEV Serial1
#define UART_SERIAL_PIN D4

void setup() {
    Serial.begin(9600);
    initSerial(UART_SERIAL_DEV, UART_SERIAL_PIN);

    pinMode(BOOT_CONFIG_PIN, INPUT);
}

unsigned long last = 0;
bool flag = true;
void loop() {
    if (digitalRead(2) == HIGH && (millis() - last) > 500) {
        last = millis();

        flag = !flag;
        if (flag) {
            Serial.print("Enabling...");
            resumeSerial();
            Serial.println(" OK");
        } else {
            Serial.print("Disabling...");
            pauseSerial();
            Serial.println(" OK");
        }
    }

    // Send TILT_UP on channel 10
    if (flag) UART_SERIAL_DEV.write("\x6a\xca\x6a");
}