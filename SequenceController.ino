#include <Arduino.h>
#include <TimerOne.h>
#include <digitalWriteFast.h>
#include <Bounce.h>

#include "SequenceController.h"

void setup()
{
  initialize();
}

void loop()
{
  for (int8_t i = 0; i < sizeof(input) / sizeof(int8_t); i++)
  {
    inputValue[i] = debouncer[i].read();
  }

  for (int8_t i = 0; i < sizeof(input) / sizeof(int8_t); i++)
  {
    debouncer[i].update();
  }

  if (debouncer[NUT_CHE_DO].rose())
  {
    switchMode();
  }
  lastModeState = inputValue[NUT_CHE_DO];

  manualMode(mode);
  automaticMode(mode);

  if (abs(millis() - oldTime) > 1500L) {
    sendReport(3);
    oldTime = millis();
  }
}
