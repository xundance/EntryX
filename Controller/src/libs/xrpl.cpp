#include "xrpl.h"

XRPL::XRPL(const String &walletAddress, const String &serverAddress, const String &root_cert)
{
    _walletAddress = walletAddress;
    _serverAddress = serverAddress;
    _root_ca = root_cert;
}

String XRPL::txRequest(const unsigned long &lli)
{
    String req = "";
    StaticJsonDocument<256> doc;

    doc["method"] = "account_tx";
    JsonObject params_0 = doc["params"].createNestedObject();
    params_0["account"] = _walletAddress;
    // params_0["binary"] = false;
    // params_0["forward"] = false;
    // params_0["ledger_index_max"] = -1;
    params_0["ledger_index_min"] = lli + 1;
    params_0["limit"] = 1;
    serializeJson(doc, req);

    return req;
}

bool XRPL::checkPayment(const unsigned long &entry_cost)
{
    DynamicJsonDocument doc(6144);
    WiFiClientSecure client;
    HTTPClient http;
    Preferences preferences;
    preferences.begin("ledger_index", false);
    unsigned long last_ledger_index = preferences.getULong("lli", 0);
    client.setCACert(_root_ca.c_str());
    http.begin(client, _serverAddress);
    http.addHeader("Content-Type", "application/json");
    int httpResponseCode = http.POST(txRequest(last_ledger_index));
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    if (httpResponseCode != 200)
        return false;
    DeserializationError error = deserializeJson(doc, http.getStream());
    if (error)
    {
        Serial.print("deserializeJson() failed: ");
        Serial.println(error.c_str());
        return false;
    }
    unsigned long amount = doc["result"]["transactions"][0]["tx"]["Amount"];
    if (!amount)
    {
        Serial.println("No transaction found.");
        return false;
    }
    String dest = doc["result"]["transactions"][0]["tx"]["Destination"];
    if (dest != _walletAddress)
    {
        Serial.println("Outgoing Transaction.");
        return false;
    }
    if (amount < entry_cost)
    {
        Serial.print("Amount to small: ");
        Serial.println(amount);
        return false;
    }
    last_ledger_index = doc["result"]["transactions"][0]["tx"]["ledger_index"];
    preferences.putULong("lli", last_ledger_index);
    preferences.end();
    Serial.println(amount);
    Serial.println(last_ledger_index);

    http.end();
    return true;
}
