/*
 * Created by SGHardwareSoftware, Feb 2026
 * 
 * Arduino sketch for ARGBController MCU 2 (Slave)
 * It handles the 8 ARGB channel ARGB light modes via NeoPixel 
 * Uses "ARGBChannel.h" class as a wrapper for NeoPixel, while packaging 16 pre-programmed ARGB modes
 * interacts with MCU 1 (main) by receiving a 8-bit value (4 bit channel, 4 bit mode) over softserial
 * 
 * compiled for 8-bit AVR ATmega MCUs i.e. ATmega168, ATmega328
 * tested working with or without 16MHz ext. crystal
 * NOTE: to run without crystal (int. 8MHz oscillator), set low fuse E2, high fuse D7 and extended fuse FD on AVRDUDE
 * and compile this sketch with internal 8 MHz setting (using MiniCore board)
 */

#include "./ARGBChannel.h"
#include "./SoftSerialBytes.h"

/*
 * softSerial uses pins 8 (RX) and 9 (TX)
 */
#define serialRxFlag_pin 10
#define ARGBChannel1_pin A0
#define ARGBChannel2_pin A1
#define ARGBChannel3_pin A2
#define ARGBChannel4_pin A3
#define ARGBChannel5_pin A4
#define ARGBChannel6_pin A5
#define ARGBChannel7_pin 2
#define ARGBChannel8_pin 3
#define ARGBLedCount 15
#define ARGBBrightness 192

SoftSerialBytes softSerial(9600);
ARGBChannel channel1(ARGBChannel1_pin, ARGBLedCount);
ARGBChannel channel2(ARGBChannel2_pin, ARGBLedCount);
ARGBChannel channel3(ARGBChannel3_pin, ARGBLedCount);
ARGBChannel channel4(ARGBChannel4_pin, ARGBLedCount);
ARGBChannel channel5(ARGBChannel5_pin, ARGBLedCount);
ARGBChannel channel6(ARGBChannel6_pin, ARGBLedCount);
ARGBChannel channel7(ARGBChannel7_pin, ARGBLedCount);
ARGBChannel channel8(ARGBChannel8_pin, ARGBLedCount);

void initARGBChannels() {
  //initalize all channel with default mode 1
  
  channel1.begin(1, 75, ARGBBrightness);
  channel2.begin(1, 75, ARGBBrightness);
  channel3.begin(1, 75, ARGBBrightness);
  channel4.begin(1, 75, ARGBBrightness);
  channel5.begin(1, 75, ARGBBrightness);
  channel6.begin(1, 75, ARGBBrightness);
  channel7.begin(1, 75, ARGBBrightness);
  channel8.begin(1, 75, ARGBBrightness);
}

void changeARGBMode(uint8_t channelVal, uint8_t modeVal) { 
  switch (channelVal) {
    case 1:
      channel1.updateARGBMode(modeVal); break;
    case 2:
      channel2.updateARGBMode(modeVal); break;
    case 3:
      channel3.updateARGBMode(modeVal); break;
    case 4:
      channel4.updateARGBMode(modeVal); break;
    case 5:
      channel5.updateARGBMode(modeVal); break;
    case 6:
      channel6.updateARGBMode(modeVal); break;
    case 7:
      channel7.updateARGBMode(modeVal); break;
    case 8:
      channel8.updateARGBMode(modeVal); break;
  }
}

void setup() {
  pinMode(serialRxFlag_pin, INPUT);
  initARGBChannels();
  softSerial.begin();
}

void loop() {
  //check for argb channel and mode changes
  if (digitalRead(serialRxFlag_pin) == HIGH) {
    byte receivedByte = softSerial.receiveByte();
    if (receivedByte != 0xFF) {
      /*
       * channel and mode numbers in MCU 1 start from 1
       * channel and mode numbers in MCU 2 start from 0
       */
      uint8_t channel = (receivedByte >> 4) + 1;
      uint8_t mode = (receivedByte & 0x0F) + 1;
      changeARGBMode(channel, mode);
    }
  }
  else {
    //run all argb modes passing the IR receiver idle state param, with their respective timers inside
    //when IR receiver is not idle, don't strip.show() for all channels
    channel1.runARGBMode();
    channel2.runARGBMode();
    channel3.runARGBMode();
    channel4.runARGBMode();
    channel5.runARGBMode();
    channel6.runARGBMode();
    channel7.runARGBMode();
    channel8.runARGBMode();
  }
}
