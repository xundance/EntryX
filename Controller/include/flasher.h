#include <Arduino.h>

class Flasher
{
private:
    int ledPin;                   // the number of the LED pin
    long OnTime;                  // milliseconds of on-time
    long OffTime;                 // milliseconds of off-time
    int ledState;                 // ledState used to set the LED
    unsigned long previousMillis; // will store last time LED was updated

public:
    Flasher(int pin, long on, long off);
    void Update();
};
// #include "flasher.h"
// Flasher f1(GREEN_LED_PIN, 100, 500);
// f1.Update();