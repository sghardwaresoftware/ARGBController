#pragma once
#ifndef CHANNELSLEDS_H
#define CHANNELSLEDS_H

#include <Arduino.h>

class ChannelsLEDs {
private:
  const uint8_t shClkPin;
  const uint8_t stClkPin;
  const uint8_t dataPin;
public:
  ChannelsLEDs(uint8_t shClkPin, uint8_t stClkPin, uint8_t dataPin) : shClkPin(shClkPin), stClkPin(stClkPin), dataPin(dataPin) {}
  
  void begin(uint8_t initChannel) {
    pinMode(shClkPin, OUTPUT);
    pinMode(stClkPin, OUTPUT);
    pinMode(dataPin, OUTPUT);

    //turn on LED of the initalized channel at the beginning
    digitalWrite(stClkPin, LOW);
    shiftOut(dataPin, shClkPin, MSBFIRST, 1 << (initChannel - 1));
    digitalWrite(stClkPin, HIGH);
  }

  void updateChannelsLEDs(uint8_t currentChannel) {
    //turn on LED of the updated channel 
    digitalWrite(stClkPin, LOW);
    shiftOut(dataPin, shClkPin, MSBFIRST, 1 << (currentChannel - 1));
    digitalWrite(stClkPin, HIGH);
  }
  
};

#endif
