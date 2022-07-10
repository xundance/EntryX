#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>

class DoorServer
{
private:
    String _serverAddress;

public:
    DoorServer(const String &serverAddress);
    bool checkPayment();
};