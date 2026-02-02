#pragma once

#ifndef SOFTSERIALBYTES_H
#define SOFTSERIALBYTES_H

#define sendByteDelay 100
#define ackByte 0xFA

#include <Arduino.h>
#include <AltSoftSerial.h>

class SoftSerialBytes {
private:
  AltSoftSerial softSerial;
  uint16_t softSerialBaud;
  uint8_t softSerialLed;

public:
  SoftSerialBytes(uint16_t softSerialBaud) : softSerialBaud(softSerialBaud) {}

  void begin() { 
    softSerial.begin(softSerialBaud);
  }

  void sendByte(byte message) {
    softSerial.write(message);
    delay(sendByteDelay);
  }

  byte receiveByte() {
    byte theByte = 0xFF;
    
    if (softSerial.available() > 0) {
      theByte = softSerial.read();
    }

    return theByte;
  }
};

#endif
