#pragma once

#ifndef IRRECEIVER_H
#define IRRECEIVER_H

#define IR_RECEIVE_ONLY
#define DECODE_NEC
#include <IRremote.hpp>

#define minReceiveInterval 1250 //minimum ms in between each ir receive

class IRReceiver {
private:
  int IRReceiverPin;
  unsigned long prevReceivedTime = 0;
  
public:
  IRReceiver(int IRReceiverPin) : IRReceiverPin(IRReceiverPin) {}
  
  void begin() {
     IrReceiver.begin(IRReceiverPin, DISABLE_LED_FEEDBACK);
     delay(100); 
     IrReceiver.resume();
  }
  
  uint16_t listenIRReceiver() {
    uint16_t command = 0xFFFF;
    
    if (IrReceiver.decode()) {
      unsigned long receivedTime = millis();
      
      if (receivedTime - prevReceivedTime >= minReceiveInterval) {
        prevReceivedTime = receivedTime;
        command = IrReceiver.decodedIRData.command;
      }
      
      IrReceiver.resume();
    }
    return command;
  }

  bool isIdle() {
    return IrReceiver.isIdle();
  }
  
};

#endif
