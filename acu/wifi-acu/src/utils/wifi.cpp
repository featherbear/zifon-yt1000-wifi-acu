#include "wifi.hpp"

#include <ArduinoJson.h>

#include "config.hpp"
#include "configurator.hpp"

namespace WifiUtils {

String lastScanResult = "[]";
static uint32_t lastScanFinishedTime = 0;

String getMACPrefix() {
    String MAC = WiFi.macAddress();
    MAC.replace(":", "");
    return MAC.substring(0, 10);
}

String getHostname() {
    return WiFi.getHostname();
}

String getSSID() {
    return WiFi.SSID();
}

void initWiFi() {
    Config::begin();

    WiFi.onStationModeDisconnected([](const WiFiEventStationModeDisconnected &event) {
        Serial.println("Disconnected from WiFi access point");
        Serial.print("WiFi lost connection. Reason: ");
        Serial.println(event.reason);
        Serial.println("Trying to Reconnect");
        // TODO: Check
        WiFi.reconnect();
    });

    WiFi.mode(WIFI_STA);
    WiFi.setHostname(("PT-" + getMACPrefix()).c_str());

    if (Config::ssid.isEmpty() || (!Config::isDHCP && (Config::ip.isEmpty() || Config::mask.isEmpty()))) {
        Serial.println("Required WiFi configuration options not set. Starting configurator...");
        Configurator::startConfigurator();
    }

    if (!Config::isDHCP) {
        IPAddress localIP;
        localIP.fromString(Config::ip);
        IPAddress subnetMask;
        subnetMask.fromString(Config::mask);

        // Ignore gateway
        WiFi.config(localIP, localIP, subnetMask);
    }

    WiFi.begin(Config::ssid.c_str(), Config::password.isEmpty() ? NULL : Config::password.c_str());
}

String discoverNetworks() {
    int n_networks_or_status = WiFi.scanComplete();

    // If already scanning, ignore
    if (n_networks_or_status == -1) return lastScanResult;

    // Scan not yet started, begin async scan
    if (n_networks_or_status == -2) {
        // If last scan was performed less than 5 seconds ago, ignore
        if (millis() - lastScanFinishedTime < 5000) return lastScanResult;

        WiFi.scanNetworks(true);
        return lastScanResult;
    }

    // 0+ networks found

    // Serial.println("Starting scan");
    // Serial.printf("Found %d networks\n", n_networks_or_status);

    if (n_networks_or_status == 0) {
        lastScanResult = "[]";
    } else {
        // Calculate size
        uint32_t docSize = 0;
        for (int i = 0; i < n_networks_or_status; i++) {
            docSize += 64 + 4;  // 64 - structure, 4 - 4-byte length
            docSize += WiFi.SSID(i).length();
            docSize += WiFi.BSSIDstr(i).length();
        }

        DynamicJsonDocument doc(docSize);
        for (int i = 0; i < n_networks_or_status; i++) {
            JsonArray apArray = doc.createNestedArray();
            apArray.add(WiFi.SSID(i));
            apArray.add(WiFi.BSSIDstr(i));
            apArray.add(WiFi.RSSI(i));
        }

        lastScanResult.clear();
        serializeJson(doc, lastScanResult);
    }

    WiFi.scanDelete();

    lastScanFinishedTime = millis();
    return lastScanResult;
}

void waitForConnect() {
    waitForConnect(NULL);
}
void waitForConnect(bool (*interrupt)()) {
    while (!isConnected()) {
        if (interrupt && interrupt()) {
            Configurator::startConfigurator();
        }
        delay(1000);
    }
}

String getIPAddress() {
    return WiFi.localIP().toString();
}

bool isConnected() {
    return WiFi.status() == WL_CONNECTED;
}
}  // namespace WifiUtils