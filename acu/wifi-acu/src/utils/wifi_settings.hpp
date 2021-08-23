#include <WString.h>

namespace WifiSettings {

extern String ssid;
extern String password;
extern bool isDHCP;
extern String ip;
extern String mask;

void begin();
void set_ssid(const char *val);
void set_password(const char *val);
void set_isDHCP(const bool val);
void set_ip(const char *val);
void set_mask(const char *val);
}  // namespace Config