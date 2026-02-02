#include "./ARGBChannel.h"
#include "./SoftSerialBytes.h"

#define serialRxFlag_pin A0
#define ARGBChannel1_pin 2
#define ARGBChannel2_pin 3
#define ARGBChannel3_pin 4
#define ARGBChannel4_pin 5
#define ARGBChannel5_pin A1
#define ARGBChannel6_pin A2
#define ARGBChannel7_pin A3
#define ARGBChannel8_pin A4
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
      uint8_t channel = receivedByte >> 4;
      uint8_t mode = receivedByte & 0x0F;
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
