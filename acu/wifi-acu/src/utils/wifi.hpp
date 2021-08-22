#include <ESP8266WiFi.h>

namespace WifiUtils {

String getHostname();
String getSSID();
String getMACPrefix();
String getIPAddress();
void initWiFi();
void waitForConnect();
void waitForConnect(bool (*)());

String discoverNetworks();

bool isConnected();

}  // namespace WifiUtils