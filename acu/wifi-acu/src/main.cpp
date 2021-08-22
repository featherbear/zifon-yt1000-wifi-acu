#include <Arduino.h>
#include <ESP8266Wifi.h>
#include <ESP8266mDNS.h>

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
        beginConfigurator();
    }

    // TODO: Connect / Host wireless
    // Try to connect to WiFi
}

// Kinda just yoinked this out of Talyte - https://github.com/featherbear/talyte/blob/8db9e89ccb8edcd62d30c070576bf882d8d47efc/src/standalone/utils/configurator.cpp#L59
void beginConfigurator() {
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