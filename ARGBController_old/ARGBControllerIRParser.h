#pragma once

#ifndef ARGBCONTROLLER_IRPARSER_H
#define ARGBCONTROLLER_IRPARSER_H

#include <Arduino.h>
#include "IRReceiver.h"

class ARGBControllerIRParser {
private:
  IRReceiver irReceiver;
public:
  enum IRCommandsConstants : uint16_t {
    prevChannel = 69, nextChannel = 71,
    prevMode = 68, nextMode = 64,
    color1 = 12, color2 = 24, color3 = 94,
    color4 = 8, color5 = 28, color6 = 90, color7 = 66
  };

  struct IRCommandEvent {
    bool changeChannel = false;
    bool changeMode = false;
    bool changeColor = false;

    uint8_t channelUpOrDown = 0;    //0 is up, 1 is down
    uint8_t modeUpOrDown = 0;       //0 is up, 1 is down
    uint8_t colorIndex = 0;         //refer to ARGBChannel.h for color index reference
  };

  ARGBControllerIRParser(uint8_t irReceiverPin) : irReceiver(irReceiverPin) {}

  void begin() {
    irReceiver.begin();
  }

  IRCommandEvent parseIRCommand() {
    IRCommandEvent ice;
    IRReceiver::IRReceiverState irs = irReceiver.listenIRReceiver();

    //receive the decoded listened IR receiver data
    //if valid and not repeated, parse the command 
    if (irs.receivedSignal && !irs.repeatedSignal) {
      uint16_t irCommand = irs.command;
      //Serial.print("IR Command is: "); Serial.println(irCommand);

      switch (irCommand) {
        case prevChannel:
          ice.changeChannel = true; ice.channelUpOrDown = 1;
          break;
        case nextChannel:
          ice.changeChannel = true; ice.channelUpOrDown = 0;
          break;
        case prevMode:
          ice.changeMode = true; ice.modeUpOrDown = 1;
          break;
        case nextMode:
          ice.changeMode = true; ice.modeUpOrDown = 0;
          break;
        case color1:
          ice.changeColor = true; ice.colorIndex = 1;
          break;
        case color2:
          ice.changeColor = true; ice.colorIndex = 2;
          break;
        case color3:
          ice.changeColor = true; ice.colorIndex = 3;
          break;
        case color4:
          ice.changeColor = true; ice.colorIndex = 4;
          break;
        case color5:
          ice.changeColor = true; ice.colorIndex = 5;
          break;
        case color6:
          ice.changeColor = true; ice.colorIndex = 6;
          break;
        case color7:
          ice.changeColor = true; ice.colorIndex = 7;
          break;
      }
    }
    return ice;
  }

  bool isIdle() {
    return irReceiver.isIdle();
  }
  
};

#endif
