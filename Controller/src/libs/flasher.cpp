#include "flasher.h"

Flasher::Flasher(int pin, long on, long off)
{
    ledPin = pin;
    pinMode(ledPin, OUTPUT);

    OnTime = on;
    OffTime = off;

    ledState = LOW;
    previousMillis = 0;
}

void Flasher::Update()
{
    unsigned long currentMillis = millis();

    if ((ledState == HIGH) && (currentMillis - previousMillis >= OnTime))
    {
        ledState = LOW;
        previousMillis = currentMillis;
        digitalWrite(ledPin, ledState);
    }
    else if ((ledState == LOW) && (currentMillis - previousMillis >= OffTime))
    {
        ledState = HIGH;
        previousMillis = currentMillis;
        digitalWrite(ledPin, ledState);
    }
}
