#include <Arduino.h>

#include "arduino-ptz/arduino-ptz.hpp"
#include "utils/configurator.hpp"
#include "utils/wifi.hpp"

#define BOOT_CONFIG_PIN D1

#define UART_SERIAL_DEV Serial1
#define UART_SERIAL_PIN D4

void beginConfigurator();

void setup() {
    Serial.begin(9600);
    initSerial(UART_SERIAL_DEV, UART_SERIAL_PIN);

    pinMode(BOOT_CONFIG_PIN, INPUT);

    // Check if the boot config pin was held down during init
    // if so, open wireless configurator
    if (digitalRead(BOOT_CONFIG_PIN) == HIGH) {
        Serial.println("Boot config pin pressed...");
        Configurator::startConfigurator();
    }

    WifiUtils::initWiFi();
    WifiUtils::waitForConnect();
}

// unsigned long last = 0;
// bool flag = true;
void loop() {
    if (digitalRead(BOOT_CONFIG_PIN) == HIGH) {
        Serial.print(".");
    }
    // delay(1000);
    // if (digitalRead(2) == HIGH && (millis() - last) > 500) {
    //     last = millis();

    //     flag = !flag;
    //     if (flag) {
    //         Serial.print("Enabling...");
    //         resumeSerial();
    //         Serial.println(" OK");
    //     } else {
    //         Serial.print("Disabling...");
    //         pauseSerial();
    //         Serial.println(" OK");
    //     }
    // }

    // // Send TILT_UP on channel 10
    // if (flag) UART_SERIAL_DEV.write("\x6a\xca\x6a");
}