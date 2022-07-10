#include <Arduino.h>
#include "doorServer.h"
#include "utils.h"

#define BAUD 115200
#define GREEN_LED_PIN 27
#define RED_LED_PIN 26
#define BUTTON_PIN 25
#define DOOR_PIN 33

const String serverAddress = "http://192.168.1.1..:8070/knock";
const String ssid = "<YOUR SSID>";
const String password = "<YOUR PASSWORD>";
DoorServer ds(serverAddress);
int button_state = LOW;

void setup()
{
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
    if (ds.checkPayment())
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
        digitalWrite(RED_LED_PIN, (on ? HIGH : LOW));
        on = !on;
      }
      digitalWrite(RED_LED_PIN, LOW);
    }
  }
}