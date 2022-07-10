#include <Arduino.h>
#include <WiFi.h>

namespace utils
{
    bool connectWifi(const String &ssid, const String &password);
}