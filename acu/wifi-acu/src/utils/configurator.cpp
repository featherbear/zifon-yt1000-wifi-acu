#include <Arduino.h>
#include <ArduinoJson.h>
#include <ESP8266mDNS.h>
#include <Hash.h>
// #include <FS.h>
// #include <spiffs/spiffs.h>
#include <ESP8266WebServer.h>

#include "configurator_data.h"
#include "wifi.hpp"

#if defined(ESP8266)
#define WebServer ESP8266WebServer
#endif

namespace Configurator {

static void loop();

static std::unique_ptr<WebServer> server = NULL;

static String getContentType(String filename);

static void setWifiAuth(const char* ssid, const char* password);
static void setWifiIPMode(bool isDHCP);
static void setWifiIPMode(bool isDHCP, const char* ip, const char* mask);

// Preferences wifiPreferences;

static void setWifiAuth(const char* ssid, const char* password) {
    // wifiPreferences.begin(NVR_KEY_WIFI, false);
    // wifiPreferences.putString("ssid", ssid);
    // wifiPreferences.putString("password", password);
    // wifiPreferences.end();
}

static void setWifiIPMode(bool isDHCP) {
    setWifiIPMode(isDHCP, NULL, NULL);
}
static void setWifiIPMode(bool isDHCP, const char* ip, const char* mask) {
    // wifiPreferences.begin(NVR_KEY_WIFI, false);
    // wifiPreferences.putBool("useDHCP", isDHCP);
    // if (!isDHCP) {
    //     wifiPreferences.putString("ip", ip);
    //     wifiPreferences.putString("mask", mask);
    // }
    // wifiPreferences.end();
}

void startConfigurator() {
    WiFi.disconnect();
    WiFi.mode(WIFI_AP);

    WiFi.softAP(WiFi.getHostname());
    IPAddress IP = WiFi.softAPIP();

    Serial.println("Starting configurator...");
    Serial.printf("SSID: %s\n", WiFi.getHostname());
    Serial.printf("IP address: %s\n", IP.toString().c_str());

    if (!MDNS.begin(WiFi.getHostname())) {
        Serial.println("Error setting up MDNS responder!");
    }

    server = std::unique_ptr<WebServer>(new WebServer(80));

    server->on(FPSTR(WWW_PATHS::DATA_NETWORKS), []() {
        if (server->method() != HTTP_GET) return server->close();

        server->send(200, FPSTR(CONTENT_TYPES::JSON), WifiUtils::discoverNetworks());
    });

    server->on(FPSTR(WWW_PATHS::DATA_CURRENT), []() {
        if (server->method() != HTTP_GET) return server->close();
        static String response("");

        if (!response.isEmpty()) {
            server->send(200, FPSTR(CONTENT_TYPES::JSON), response);
            return;
        }

        StaticJsonDocument<1024> doc;

        // {
        //     wifiPreferences.begin(NVR_KEY_WIFI, true);
        //     doc["ssid"] = wifiPreferences.getString("ssid");
        //     doc["password"] = wifiPreferences.getString("password");
        //     doc["mode"] = wifiPreferences.getBool("useDHCP", true) ? "dhcp" : "static";
        //     doc["static_ip"] = wifiPreferences.getString("ip");
        //     doc["static_mask"] = wifiPreferences.getString("mask");
        //     wifiPreferences.end();
        // }

        serializeJson(doc, response);
        server->send(200, FPSTR(CONTENT_TYPES::JSON), response);
    });

    server->on(FPSTR(WWW_PATHS::SET_CONFIG), []() {
        if (server->method() != HTTP_POST) return server->close();

        setWifiAuth(
            server->arg(F("ssid")).c_str(),
            server->arg(F("password")).c_str());

        setWifiIPMode(server->arg(F("mode")).equals("dhcp"),
                      server->arg(F("static_ip")).c_str(),
                      server->arg(F("static_mask")).c_str());

        server->send(200);
        Serial.println("Changes made.. flushing...");
        delay(1000);
        server->close();
        Serial.println("Restarting");
        ESP.restart();
    });

    // TODO:
    // SPIFFS.begin();
    // server->serveStatic("/", SPIFFS, "/");

    server->onNotFound([]() {
        String path = server->uri();

        Serial.println("handleFileRead: " + path);
        if (path.endsWith("/")) path += "index.html";

        if (!false) {
            // if (!SPIFFS.exists(path)) {
            Serial.println("\tFile Not Found");
            server->send(404, FPSTR(CONTENT_TYPES::PLAIN), "404: Not Found");
            return;
        }

        // TODO:
        // File file = SPIFFS.open(path, "r");
        // server->streamFile(file, getContentType(path));
        // file.close();
    });

    server->begin();

    WifiUtils::discoverNetworks();
    while (true) server->handleClient();
}

static String getContentType(String filename) {
    if (filename.endsWith(".html"))
        return FPSTR(CONTENT_TYPES::HTML);
    else if (filename.endsWith(".css"))
        return FPSTR(CONTENT_TYPES::CSS);
    else if (filename.endsWith(".js"))
        return FPSTR(CONTENT_TYPES::JS);
    else if (filename.endsWith(".ico"))
        return FPSTR(CONTENT_TYPES::ICO);

    // Unknown?
    return FPSTR(CONTENT_TYPES::PLAIN);
}

}  // namespace Configurator