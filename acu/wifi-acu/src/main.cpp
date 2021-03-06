#include <Arduino.h>

#include "arduino-ptz/arduino-ptz.hpp"
#include "utils/configurator.hpp"
#include "utils/wifi.hpp"
#include "ws/server.hpp"
#include "www/server.hpp"

#define BOOT_CONFIG_PIN D1

#define UART_SERIAL_DEV Serial1
#define UART_SERIAL_PIN D4

void beginConfigurator();

void setup() {
    Serial.begin(9600);
    ArduinoPTZ::initSerial(UART_SERIAL_DEV, UART_SERIAL_PIN);

    // phy: Attach button K2 to BOOT_CONFIG_PIN
    pinMode(BOOT_CONFIG_PIN, INPUT_PULLUP);

    // Check if the boot config pin was held down during init
    // if so, open wireless configurator
    if (digitalRead(BOOT_CONFIG_PIN) == LOW) {
        Serial.println("Boot config pin pressed...");
        Configurator::startConfigurator();
        // Infinite loop
    }

    PT_WWW::init();

    WifiUtils::initWiFi();

    Serial.printf("Connecting to %s...\n", WifiUtils::getSSID().c_str());

    WifiUtils::waitForConnect();

    PT_WWW::begin();

    PT_WS::setDataHandler([](uint8_t *payload, size_t length) {
        if (length < 1) return;
        ArduinoPTZ::process(payload[0]);
    });

    PT_WS::begin();
    Serial.printf("Connected - IP %s (%s)\n", WifiUtils::getIPAddress().c_str(), WifiUtils::getHostname().c_str());
}

void loop() {
    PT_WWW::tick();
    PT_WS::tick();

    uint8_t *buffer = ArduinoPTZ::getSend();
    if (buffer) UART_SERIAL_DEV.write(buffer, 3);
}