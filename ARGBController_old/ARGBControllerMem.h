#pragma once

#ifndef ARGBCONTROLLER_MEM_H
#define ARGBCONTROLLER_MEM_H

#include <EEPROM.h>

#define numberOfChannels 8
#define firstModeValueAddr 0
#define firstColorValueAddr 8

class ARGBControllerMem {
private:
  const byte defaultAddrValue = 0x7F;
  
public:
  struct ARGBInitValues {
    uint8_t modeValues[numberOfChannels];
    uint8_t colorValues[numberOfChannels];
  };

  ARGBInitValues begin() {
    //read eeprom addresses for mode and color for values
    //if they are default, set that mode/color value to pre-default values
    //else, set that mode/color value as it was. 

    //to reset eeprom, use avrdudess to reflash "ARGBControllerMemDefault.eep" uint8_to the eeprom
    ARGBInitValues initValues;

    //read mode values from addr 0 to 7
    for (uint8_t modeValAddr = firstModeValueAddr; modeValAddr < (firstModeValueAddr + numberOfChannels); modeValAddr++) {
      byte value = EEPROM.read(modeValAddr);
      
      if (value == defaultAddrValue) {
        initValues.modeValues[modeValAddr - firstModeValueAddr] = 1; //value at this address is default, so set mode to default 1 
      }
      else {
        initValues.modeValues[modeValAddr - firstModeValueAddr] = value; //value is previously stored at this address, so set the corresponding mode
      }
    }

    //read color values from addr 8 to 15
    for (uint8_t colorValAddr = firstColorValueAddr; colorValAddr < (firstColorValueAddr + numberOfChannels); colorValAddr++) {
      byte value = EEPROM.read(colorValAddr);
      
      if (value == defaultAddrValue) {
        initValues.colorValues[colorValAddr - firstColorValueAddr] = 1; //value at this address is default, so set color to default 1 (RED)
      }
      else {
        initValues.colorValues[colorValAddr - firstColorValueAddr] = value; //value is previously stored at this address, so set the corresponding color
      }
    }

    return initValues;
  }

  void updateMemory(uint8_t valAddr, uint8_t value) {
    EEPROM.update(valAddr, value);
  }
  
};

#endif
