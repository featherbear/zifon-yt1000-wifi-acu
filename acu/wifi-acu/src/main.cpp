#include <Arduino.h>
#include "arduino-ptz/arduino-ptz.hpp"

#define BOOT_CONFIG_PIN 2

void setup() {
    Serial.begin(9600);
    initSerial(Serial2, /* SERIAL2_PIN */ 17, U2TXD_OUT_IDX);

    // Pin D2
    pinMode(2, INPUT);
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
    if (flag) Serial2.write("\x6a\xca\x6a");
}