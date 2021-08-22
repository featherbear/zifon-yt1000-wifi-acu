#include <LittleFS.h>

bool hasInit = false;

namespace Config {

String ssid;
String password;
bool isDHCP;
String ip;
String mask;

#define __filepath_ssid "/cfg/ssid"
#define __filepath_password "/cfg/password"
#define __filepath_isDHCP "/cfg/isDHCP"
#define __filepath_ip "/cfg/ip"
#define __filepath_mask "/cfg/mask"

void begin() {
    if (hasInit) return;

    LittleFS.begin();

    if (!LittleFS.exists("/cfg/")) {
        LittleFS.mkdir("/cfg/");
    }

    File f;

    if (LittleFS.exists(__filepath_ssid)) {
        f = LittleFS.open(__filepath_ssid, "r");
        ssid = f.readString();
        f.close();
    } else {
        ssid = String();
    }

    if (LittleFS.exists(__filepath_password)) {
        f = LittleFS.open(__filepath_password, "r");
        password = f.readString();
        f.close();
    } else {
        password = String();
    }

    if (LittleFS.exists(__filepath_isDHCP)) {
        f = LittleFS.open(__filepath_isDHCP, "r");
        isDHCP = !f.readString().equals("false");
        f.close();
    } else {
        isDHCP = true;
    }

    if (LittleFS.exists(__filepath_ip)) {
        f = LittleFS.open(__filepath_ip, "r");
        ip = f.readString();
        f.close();
    } else {
        ip = String();
    }

    if (LittleFS.exists(__filepath_mask)) {
        f = LittleFS.open(__filepath_mask, "r");
        mask = f.readString();
        f.close();
    } else {
        mask = String();
    }

    hasInit = true;
}

void set_ssid(const char *val) {
    File f = LittleFS.open(__filepath_ssid, "w");
    f.write(val);
    f.close();
    ssid = String(val);
}
void set_password(const char *val) {
    File f = LittleFS.open(__filepath_password, "w");
    f.write(val);
    f.close();
    password = String(val);
}
void set_isDHCP(const bool val) {
    File f = LittleFS.open(__filepath_isDHCP, "w");
    f.write(val ? "true" : "false");
    f.close();
    isDHCP = val;
}
void set_ip(const char *val) {
    File f = LittleFS.open(__filepath_ip, "w");
    f.write(val);
    f.close();
    ip = String(val);
}
void set_mask(const char *val) {
    File f = LittleFS.open(__filepath_mask, "w");
    f.write(val);
    f.close();
    mask = String(val);
}
}  // namespace Config