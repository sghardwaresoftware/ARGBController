/*IMPORTANT!! 
to reset eeprom, use avrdudess to reflash 
"ARGBControllerMemDefault.eep" uint8_to the eeprom
*/

#pragma once

#ifndef ARGBCONTROLLER_MEM_H
#define ARGBCONTROLLER_MEM_H

#include <EEPROM.h>

#define numberOfChannels 8
#define firstModeValueAddr 0

class ARGBControllerMem {
private:
  const byte defaultAddrValue = 0x7F;
  
public:
  struct ARGBInitValues {
    uint8_t modeValues[numberOfChannels];
  };

  ARGBInitValues begin() {
    //read eeprom addresses for mode values
    //if they are default, set that mode value to pre-default values
    //else, set that mode value as it was. 

    ARGBInitValues initValues;

    //read mode values from addr 0 to 7
    for (uint8_t modeValAddr = firstModeValueAddr; modeValAddr < (firstModeValueAddr + numberOfChannels); modeValAddr++) {
      byte value = EEPROM.read(modeValAddr);
      
      if (value == defaultAddrValue) {
        initValues.modeValues[modeValAddr - firstModeValueAddr] = 0; //value at this address is default, so set to 0 (first mode) 
      }
      else {
        initValues.modeValues[modeValAddr - firstModeValueAddr] = value; //value is previously stored at this address, so set the corresponding mode
      }
    }

    return initValues;
  }

  void updateMemory(uint8_t valAddr, uint8_t value) {
    EEPROM.update(valAddr, value);
  }
  
};

#endif
