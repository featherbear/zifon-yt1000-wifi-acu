#include <WString.h>

namespace Config {

static String ssid;
static String password;
static bool isDHCP;
static String ip;
static String mask;

void begin();
void set_ssid(const char *val);
void set_password(const char *val);
void set_isDHCP(const bool val);
void set_ip(const char *val);
void set_mask(const char *val);
}  // namespace Config