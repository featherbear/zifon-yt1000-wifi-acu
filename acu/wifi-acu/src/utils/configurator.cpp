#include <Arduino.h>
#include <ArduinoJson.h>

#include "../www/server.hpp"
#include "../www/www.hpp"
#include "configurator_data.h"
#include "wifi.hpp"
#include "wifi_settings.hpp"

#if defined(ESP8266)
#include <ESP8266mDNS.h>
#else
#include <mDNS.h>
#endif

namespace Configurator {

static void setWifiAuth(const char* ssid, const char* password);
static void setWifiIPMode(bool isDHCP);
static void setWifiIPMode(bool isDHCP, const char* ip, const char* mask);

static void setWifiAuth(const char* ssid, const char* password) {
    WifiSettings::set_ssid(ssid);
    WifiSettings::set_password(password);
}

static void setWifiIPMode(bool isDHCP) {
    setWifiIPMode(isDHCP, NULL, NULL);
}

static void setWifiIPMode(bool isDHCP, const char* ip, const char* mask) {
    WifiSettings::set_isDHCP(isDHCP);

    if (!isDHCP) {
        WifiSettings::set_ip(ip);
        WifiSettings::set_mask(mask);
    }
}

void startConfigurator() {
    WifiSettings::begin();

    WiFi.disconnect();
    WiFi.mode(WIFI_AP_STA);
    WiFi.setHostname(("PT-" + WifiUtils::getMACPrefix()).c_str());

    WiFi.softAP(WiFi.getHostname());
    IPAddress IP = WiFi.softAPIP();

    if (!MDNS.begin(WiFi.getHostname())) {
        Serial.println("Error setting up MDNS responder!");
    }

    Serial.println("Starting configurator...");
    Serial.printf("SSID: %s\n", WiFi.getHostname());
    Serial.printf("IP address: %s\n", IP.toString().c_str());

    PT_WWW::setMode("config");
    PT_WWW::init();

    PT_WWW::server->on(FPSTR(WWW_PATHS::DATA_NETWORKS), []() {
        if (PT_WWW::server->method() != HTTP_GET) return PT_WWW::server->close();

        PT_WWW::server->send(200, FPSTR(CONTENT_TYPES::JSON), WifiUtils::discoverNetworks());
    });

    PT_WWW::server->on(FPSTR(WWW_PATHS::DATA_CURRENT), []() {
        if (PT_WWW::server->method() != HTTP_GET) return PT_WWW::server->close();
        static String response("");

        if (!response.isEmpty()) {
            PT_WWW::server->send(200, FPSTR(CONTENT_TYPES::JSON), response);
            return;
        }

        StaticJsonDocument<1024> doc;

        {
            doc["ssid"] = WifiSettings::ssid;
            doc["password"] = WifiSettings::password;
            doc["mode"] = WifiSettings::isDHCP ? "dhcp" : "static";
            doc["static_ip"] = WifiSettings::ip;
            doc["static_mask"] = WifiSettings::mask;
        }

        serializeJson(doc, response);
        PT_WWW::server->send(200, FPSTR(CONTENT_TYPES::JSON), response);
    });

    PT_WWW::server->on(FPSTR(WWW_PATHS::SET_CONFIG), []() {
        if (PT_WWW::server->method() != HTTP_POST) return PT_WWW::server->close();

        setWifiAuth(
            PT_WWW::server->arg(F("ssid")).c_str(),
            PT_WWW::server->arg(F("password")).c_str());

        setWifiIPMode(PT_WWW::server->arg(F("mode")).equals("dhcp"),
                      PT_WWW::server->arg(F("static_ip")).c_str(),
                      PT_WWW::server->arg(F("static_mask")).c_str());

        PT_WWW::server->send(200);
        Serial.println("Changes made.. flushing...");
        delay(1000);
        PT_WWW::server->close();
        Serial.println("Restarting");
        ESP.restart();
    });

    WifiUtils::discoverNetworks();

    PT_WWW::begin();
    PT_WWW::loop();
}

}  // namespace Configurator