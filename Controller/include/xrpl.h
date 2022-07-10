#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <Preferences.h>

class XRPL
{
private:
    String _walletAddress;
    String _serverAddress;
    String _root_ca;

    String txRequest(const unsigned long &lli);

public:
    XRPL(const String &walletAddress, const String &serverAddress, const String &root_cert);
    bool checkPayment(const unsigned long &entry_cost);
};