#include "./Include/ARGBChannel.h"
#include "./Include/ARGBControllerMem.h"

#define numberOfChannels 8
#define ARGBLedCount 15
#define ARGBBrightness 192

#define ARGBChannel1_pin 2
#define ARGBChannel2_pin 3
#define ARGBChannel3_pin 4
#define ARGBChannel4_pin 5
#define ARGBChannel5_pin 6
#define ARGBChannel6_pin 7
#define ARGBChannel7_pin 8
#define ARGBChannel8_pin 9

#define channelVal_bit2_pin   A0
#define channelVal_bit1_pin   A1
#define channelVal_bit0_pin   A2
#define modeVal_bit3_pin      A3
#define modeVal_bit2_pin      A4
#define modeVal_bit1_pin      A5
#define modeVal_bit0_pin      A6

ARGBChannel channel1(ARGBChannel1_pin, ARGBLedCount);
ARGBChannel channel2(ARGBChannel2_pin, ARGBLedCount);
ARGBChannel channel3(ARGBChannel3_pin, ARGBLedCount);
ARGBChannel channel4(ARGBChannel4_pin, ARGBLedCount);
ARGBChannel channel5(ARGBChannel5_pin, ARGBLedCount);
ARGBChannel channel6(ARGBChannel6_pin, ARGBLedCount);
ARGBChannel channel7(ARGBChannel7_pin, ARGBLedCount);
ARGBChannel channel8(ARGBChannel8_pin, ARGBLedCount);

ARGBControllerMem ARGBMemory;

uint8_t currentChannel;
uint8_t prevModeVal;

void initARGBChannels() {
  //initalize the argb channel with the initialized mode, color index and pixel interval 
  //refer to ARGBChannel.h for color index reference

  ARGBControllerMem::ARGBInitValues initValues = ARGBMemory.begin();
  
  channel1.begin(initValues.modeValues[0], 75, ARGBBrightness);
  channel2.begin(initValues.modeValues[1], 75, ARGBBrightness);
  channel3.begin(initValues.modeValues[2], 75, ARGBBrightness);
  channel4.begin(initValues.modeValues[3], 75, ARGBBrightness);
  channel5.begin(initValues.modeValues[4], 75, ARGBBrightness);
  channel6.begin(initValues.modeValues[5], 75, ARGBBrightness);
  channel7.begin(initValues.modeValues[6], 75, ARGBBrightness);
  channel8.begin(initValues.modeValues[7], 75, ARGBBrightness);
}

void changeARGBMode(uint8_t modeVal) { 
  //changing mode triggered from button/IR receiver event; can change value by either method, cyclingValue or assigning value
  //refer to ARGBChannel.h updateValueMethods and cycleValueDirection enum for parameters' reference
  
  switch (currentChannel) {
    case 1:
      channel1.updateARGBMode(modeVal);
      ARGBMemory.updateMemory(0, modeVal);
      break;
    case 2:
      channel2.updateARGBMode(modeVal);
      ARGBMemory.updateMemory(1, modeVal);
      break;
    case 3:
      channel3.updateARGBMode(modeVal);
      ARGBMemory.updateMemory(2, modeVal);
      break;
    case 4:
      channel4.updateARGBMode(modeVal);
      ARGBMemory.updateMemory(3, modeVal);
      break;
    case 5:
      channel5.updateARGBMode(modeVal);
      ARGBMemory.updateMemory(4, modeVal);
      break;
    case 6:
      channel6.updateARGBMode(modeVal);
      ARGBMemory.updateMemory(5, modeVal);
      break;
    case 7:
      channel7.updateARGBMode(modeVal);
      ARGBMemory.updateMemory(6, modeVal);
      break;
    case 8:
      channel8.updateARGBMode(modeVal);
      ARGBMemory.updateMemory(7, modeVal);
      break;
  }
}

void setCurrentChannel() {
  bool channelVal_bit2 = digitalRead(channelVal_bit2_pin);
  bool channelVal_bit1 = digitalRead(channelVal_bit1_pin);
  bool channelVal_bit0 = digitalRead(channelVal_bit0_pin);
  currentChannel = 4*channelVal_bit2 + 2*channelVal_bit1 + channelVal_bit0 + 1;
}

uint8_t setCurrentMode() {
  bool modeVal_bit3 = digitalRead(modeVal_bit3_pin);
  bool modeVal_bit2 = digitalRead(modeVal_bit2_pin);
  bool modeVal_bit1 = digitalRead(modeVal_bit1_pin);
  bool modeVal_bit0 = digitalRead(modeVal_bit0_pin);
  return 8*modeVal_bit3 + 4*modeVal_bit2 + 2*modeVal_bit1 + modeVal_bit0 + 1;
}

void setup() {
  initARGBChannels();
  prevModeVal = 0;
  pinMode(channelVal_bit2_pin, INPUT); pinMode(channelVal_bit1_pin, INPUT); pinMode(channelVal_bit0_pin, INPUT); 
  pinMode(modeVal_bit3_pin, INPUT); pinMode(modeVal_bit2_pin, INPUT); pinMode(modeVal_bit1_pin, INPUT); pinMode(modeVal_bit0_pin, INPUT); 
}

void loop() {
  //check for argb channel and mode changes
  setCurrentChannel();
  uint8_t newModeVal = setCurrentMode();

  if (newModeVal != prevModeVal) {
    changeARGBMode(newModeVal);
    prevModeVal = newModeVal;
  }

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
