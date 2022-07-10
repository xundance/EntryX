#include "utils.h"

bool utils::connectWifi(const String &ssid, const String &password)
{
    int counter = 0;
    WiFi.begin(ssid.c_str(), password.c_str());
    Serial.print("Connecting");
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
        counter++;
        if (counter > 60)
        {
            Serial.println("Timeout!");
            return false;
        }
    }
    Serial.println("");
    Serial.print("Connected to WiFi network with IP Address: ");
    Serial.println(WiFi.localIP());
    return true;
}
