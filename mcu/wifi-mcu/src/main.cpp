#include <Arduino.h>

#define SERIAL2_PIN 17

void setup() {
    Serial.begin(9600);
    Serial2.begin(9600);

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
            // // Doing a serial2.begin is slow...
            // Serial2.begin(9600);

            // esp32-hal-uart::uartAttachTx
            pinMode(SERIAL2_PIN, OUTPUT);
            pinMatrixOutAttach(SERIAL2_PIN, U2TXD_OUT_IDX, false, false);

            Serial.println(" OK");
        } else {
            Serial.print("Disabling...");
            // // Doing a serial2.begin is slow...
            // Serial2.end();

            // esp32-hal-uart::uartDetachTx
            pinMode(SERIAL2_PIN, INPUT);
            pinMatrixOutDetach(SERIAL2_PIN, false, false);

            Serial.println(" OK");
        }
    }

    // Send TILT_UP on channel 10
    if (flag) Serial2.write("\x6a\xca\x6a");
}