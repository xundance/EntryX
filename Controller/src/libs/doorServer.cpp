#include "doorServer.h"

DoorServer::DoorServer(const String &serverAddress)
{
    _serverAddress = serverAddress;
};

bool DoorServer::checkPayment()
{
    WiFiClient client;
    HTTPClient http;
    http.begin(client, _serverAddress);
    int httpResponseCode = http.GET();
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    String data = http.getString();
    Serial.println("HTTP Response: " + data);
    http.end();
    return httpResponseCode == 200 ? true : false;
}
