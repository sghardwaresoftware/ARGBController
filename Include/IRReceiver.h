#pragma once

#ifndef IRRECEIVER_H
#define IRRECEIVER_H

#define IR_RECEIVE_ONLY
#define DECODE_NEC
#include <IRremote.hpp>

class IRReceiver {
private:
  int IRReceiverPin;
public:
  struct IRReceiverState {
    bool receivedSignal = false;
    bool repeatedSignal = false;
    decode_type_t protocol = 0;
    uint16_t command = 0;
    IRRawDataType rawData = 0;
  };
  
  IRReceiver(int IRReceiverPin) : IRReceiverPin(IRReceiverPin) {}
  
  void begin() {
     IrReceiver.begin(IRReceiverPin, DISABLE_LED_FEEDBACK);
  }
  
  IRReceiverState listenIRReceiver() {
    IRReceiverState irs;
    
    if (IrReceiver.decode() && !IrReceiver.decodedIRData.protocol == UNKNOWN) {
      irs.receivedSignal = true;
      irs.protocol = IrReceiver.decodedIRData.protocol;
      irs.command = IrReceiver.decodedIRData.command;
      irs.rawData = IrReceiver.decodedIRData.decodedRawData;
      
      if (IrReceiver.decodedIRData.flags & IRDATA_FLAGS_IS_REPEAT) {
          irs.repeatedSignal = true;
      }

      IrReceiver.resume();
    }

    return irs;
  }

  bool isIdle() {
    return IrReceiver.isIdle();
  }
  
};

#endif
