#include <Arduino.h>
#include "xrpl.h"
#include "utils.h"

#define ENTRY_COST 1 // in drops
#define BAUD 115200
#define GREEN_LED_PIN 27
#define RED_LED_PIN 26
#define BUTTON_PIN 25
#define DOOR_PIN 33

const String ssid = "<YOUR SSID>";
const String password = "<YOUR PASSWORD>";
const String walletAddress = "r...";
const String serverAddress = "https://xrplcluster.com/";
const String root_ca =
    "-----BEGIN CERTIFICATE-----\n"
    "MIIDdzCCAl+gAwIBAgIEAgAAuTANBgkqhkiG9w0BAQUFADBaMQswCQYDVQQGEwJJ\n"
    "RTESMBAGA1UEChMJQmFsdGltb3JlMRMwEQYDVQQLEwpDeWJlclRydXN0MSIwIAYD\n"
    "VQQDExlCYWx0aW1vcmUgQ3liZXJUcnVzdCBSb290MB4XDTAwMDUxMjE4NDYwMFoX\n"
    "DTI1MDUxMjIzNTkwMFowWjELMAkGA1UEBhMCSUUxEjAQBgNVBAoTCUJhbHRpbW9y\n"
    "ZTETMBEGA1UECxMKQ3liZXJUcnVzdDEiMCAGA1UEAxMZQmFsdGltb3JlIEN5YmVy\n"
    "VHJ1c3QgUm9vdDCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBAKMEuyKr\n"
    "mD1X6CZymrV51Cni4eiVgLGw41uOKymaZN+hXe2wCQVt2yguzmKiYv60iNoS6zjr\n"
    "IZ3AQSsBUnuId9Mcj8e6uYi1agnnc+gRQKfRzMpijS3ljwumUNKoUMMo6vWrJYeK\n"
    "mpYcqWe4PwzV9/lSEy/CG9VwcPCPwBLKBsua4dnKM3p31vjsufFoREJIE9LAwqSu\n"
    "XmD+tqYF/LTdB1kC1FkYmGP1pWPgkAx9XbIGevOF6uvUA65ehD5f/xXtabz5OTZy\n"
    "dc93Uk3zyZAsuT3lySNTPx8kmCFcB5kpvcY67Oduhjprl3RjM71oGDHweI12v/ye\n"
    "jl0qhqdNkNwnGjkCAwEAAaNFMEMwHQYDVR0OBBYEFOWdWTCCR1jMrPoIVDaGezq1\n"
    "BE3wMBIGA1UdEwEB/wQIMAYBAf8CAQMwDgYDVR0PAQH/BAQDAgEGMA0GCSqGSIb3\n"
    "DQEBBQUAA4IBAQCFDF2O5G9RaEIFoN27TyclhAO992T9Ldcw46QQF+vaKSm2eT92\n"
    "9hkTI7gQCvlYpNRhcL0EYWoSihfVCr3FvDB81ukMJY2GQE/szKN+OMY3EU/t3Wgx\n"
    "jkzSswF07r51XgdIGn9w/xZchMB5hbgF/X++ZRGjD8ACtPhSNzkE1akxehi/oCr0\n"
    "Epn3o0WC4zxe9Z2etciefC7IpJ5OCBRLbf1wbWsaY71k5h+3zvDyny67G7fyUIhz\n"
    "ksLi4xaNmjICq44Y3ekQEe5+NauQrz4wlHrQMz2nZQ/1/I6eYs9HRCwBXbsdtTLS\n"
    "R9I4LtD+gdwyah617jzV/OeBHRnDJELqYzmp\n"
    "-----END CERTIFICATE-----\n";
XRPL x(walletAddress, serverAddress, root_ca);
int button_state = LOW;

void setup()
{
  // Preferences preferences;
  // preferences.begin("ledger_index", false);
  // preferences.putULong("lli", 0);
  // preferences.end();

  Serial.begin(BAUD);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(DOOR_PIN, OUTPUT);
  pinMode(RED_LED_PIN, OUTPUT);
  pinMode(GREEN_LED_PIN, OUTPUT);
  digitalWrite(RED_LED_PIN, HIGH);
  while (!utils::connectWifi(ssid, password))
    delay(5000);
  digitalWrite(RED_LED_PIN, LOW);
}

void loop()
{
  button_state = digitalRead(BUTTON_PIN);
  if (button_state == LOW)
  {
    if (WiFi.status() != WL_CONNECTED)
    {
      digitalWrite(RED_LED_PIN, HIGH);
      utils::connectWifi(ssid, password);
      digitalWrite(RED_LED_PIN, LOW);
    }

    digitalWrite(GREEN_LED_PIN, HIGH);
    digitalWrite(RED_LED_PIN, HIGH);
    if (x.checkPayment(ENTRY_COST))
    {
      digitalWrite(RED_LED_PIN, LOW);
      digitalWrite(DOOR_PIN, HIGH);
      delay(6000);
      digitalWrite(DOOR_PIN, LOW);
      digitalWrite(GREEN_LED_PIN, LOW);
    }
    else
    {
      digitalWrite(GREEN_LED_PIN, LOW);
      bool on = LOW;
      for (int i = 0; i < 16; i++)
      {
        delay(250);
        digitalWrite(RED_LED_PIN, (on ? LOW : HIGH));
        on = on ? LOW : HIGH;
      }
      digitalWrite(RED_LED_PIN, LOW);
    }
  }
}