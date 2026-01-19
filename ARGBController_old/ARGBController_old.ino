#include "ARGBChannel.h"
#include "ARGBControllerMem.h"
#include "PushButton.h"
#include "ARGBControllerIRParser.h"
#include "ChannelsLEDs.h"

#define numberOfChannels 8
#define ARGBLedCount 12

#define ARGBChannel1_pin 2
#define ARGBChannel2_pin 3
#define ARGBChannel3_pin 4
#define ARGBChannel4_pin 5
#define ARGBChannel5_pin 6
#define ARGBChannel6_pin 7
#define ARGBChannel7_pin 8
#define ARGBChannel8_pin 9

PushButton argbButton(A0);                    //pin A0 is the push button switch
ARGBControllerIRParser argbIRReceiver(A4);    //pin A4 is the IR receiver; uses the IRReceiver.h class
ChannelsLEDs channelLEDs(A1, A2, A3);         //pins A1, A2, A3 are the LEDs' shift register sh_clk, st_clk and data

ARGBChannel channel1(ARGBChannel1_pin, ARGBLedCount);
ARGBChannel channel2(ARGBChannel2_pin, ARGBLedCount);
ARGBChannel channel3(ARGBChannel3_pin, ARGBLedCount);
ARGBChannel channel4(ARGBChannel4_pin, ARGBLedCount);
ARGBChannel channel5(ARGBChannel5_pin, ARGBLedCount);
ARGBChannel channel6(ARGBChannel6_pin, ARGBLedCount);
ARGBChannel channel7(ARGBChannel7_pin, ARGBLedCount);
ARGBChannel channel8(ARGBChannel8_pin, ARGBLedCount);

ARGBControllerMem memory;

uint8_t channelToSetting = 1;

void initARGBChannels(ARGBControllerMem::ARGBInitValues initValues) {
  //initalize the argb channel with the initialized mode, color index and pixel interval 
  //refer to ARGBChannel.h for color index reference
  
  channel1.begin(initValues.modeValues[0], initValues.colorValues[0], 75);
  channel2.begin(initValues.modeValues[1], initValues.colorValues[1], 75);
  channel3.begin(initValues.modeValues[2], initValues.colorValues[2], 75);
  channel4.begin(initValues.modeValues[3], initValues.colorValues[3], 75);
  channel5.begin(initValues.modeValues[4], initValues.colorValues[4], 75);
  channel6.begin(initValues.modeValues[5], initValues.colorValues[5], 75);
  channel7.begin(initValues.modeValues[6], initValues.colorValues[6], 75);
  channel8.begin(initValues.modeValues[7], initValues.colorValues[7], 75);
}

void changeARGBMode(uint8_t changeValueMethod, uint8_t valueToChange) { 
  //changing mode triggered from button/IR receiver event; can change value by either method, cyclingValue or assigning value
  //refer to ARGBChannel.h updateValueMethods and cycleValueDirection enum for parameters' reference
  uint8_t newMode;
  
  switch (channelToSetting) {
    case 1:
      newMode = channel1.updateARGBMode(changeValueMethod, valueToChange);
      memory.updateMemory(0, newMode);
      break;
    case 2:
      newMode = channel2.updateARGBMode(changeValueMethod, valueToChange);
      memory.updateMemory(1, newMode);
      break;
    case 3:
      newMode = channel3.updateARGBMode(changeValueMethod, valueToChange);
      memory.updateMemory(2, newMode);
      break;
    case 4:
      newMode = channel4.updateARGBMode(changeValueMethod, valueToChange);
      memory.updateMemory(3, newMode);
      break;
    case 5:
      newMode = channel5.updateARGBMode(changeValueMethod, valueToChange);
      memory.updateMemory(4, newMode);
      break;
    case 6:
      newMode = channel6.updateARGBMode(changeValueMethod, valueToChange);
      memory.updateMemory(5, newMode);
      break;
    case 7:
      newMode = channel7.updateARGBMode(changeValueMethod, valueToChange);
      memory.updateMemory(6, newMode);
      break;
    case 8:
      newMode = channel8.updateARGBMode(changeValueMethod, valueToChange);
      memory.updateMemory(7, newMode);
      break;
  }
}

void changeARGBColor(uint8_t changeValueMethod, uint8_t valueToChange) {
  //changing color triggered from button/IR receiver event; can change value by either method, cyclingValue or assigning value
  //refer to ARGBChannel.h updateValueMethods and cycleValueDirection enum for parameters' reference
  uint8_t newColorIndex;
  
  switch (channelToSetting) {
    case 1:
      newColorIndex = channel1.updateARGBColor(changeValueMethod, valueToChange);
      memory.updateMemory(8, newColorIndex);
      break;
    case 2:
      newColorIndex = channel2.updateARGBColor(changeValueMethod, valueToChange);
      memory.updateMemory(9, newColorIndex);
      break;
    case 3:
      newColorIndex = channel3.updateARGBColor(changeValueMethod, valueToChange);
      memory.updateMemory(10, newColorIndex);
      break;
    case 4:
      newColorIndex = channel4.updateARGBColor(changeValueMethod, valueToChange);
      memory.updateMemory(11, newColorIndex);
      break;
    case 5:
      newColorIndex = channel5.updateARGBColor(changeValueMethod, valueToChange);
      memory.updateMemory(12, newColorIndex);
      break;
    case 6:
      newColorIndex = channel6.updateARGBColor(changeValueMethod, valueToChange);
      memory.updateMemory(13, newColorIndex);
      break;
    case 7:
      newColorIndex = channel7.updateARGBColor(changeValueMethod, valueToChange);
      memory.updateMemory(14, newColorIndex);
      break;
    case 8:
      newColorIndex = channel8.updateARGBColor(changeValueMethod, valueToChange);
      memory.updateMemory(15, newColorIndex);
      break;
  }
}

void changeARGBChannel(uint8_t direction) { 
  //changing channel triggered from button/IR receiver event; can change value ONLY by cyclingValue 
  //refer to ARGBChannel.h updateValueMethods and cycleValueDirection enum for parameters' reference
  if (direction == ARGBChannel::cycleValueDirection::increaseValue) {
    if (channelToSetting == numberOfChannels) { channelToSetting = 1; }
    else { channelToSetting += 1; }
  }
  else if (direction == ARGBChannel::cycleValueDirection::decreaseValue) {
    if (channelToSetting == 1) { channelToSetting = numberOfChannels; }
    else { channelToSetting -= 1; }
  }
  
  channelLEDs.updateChannelsLEDs(channelToSetting); //update the channel indicator leds
}

void setup() {
  //Serial.begin(9600);
  ARGBControllerMem::ARGBInitValues initValues = memory.begin();
  initARGBChannels(initValues);
  argbButton.begin();
  argbIRReceiver.begin();
  channelLEDs.begin(channelToSetting);
}

void loop() {
  //listen for pushbutton event
  PushButton::ButtonEvent be = argbButton.listenButton(); 

  if (be == PushButton::shortPressed) { 
    //short button press to move to next mode
    changeARGBMode(ARGBChannel::updateValueMethods::cycleValue, ARGBChannel::cycleValueDirection::increaseValue);
  }
  else if (be == PushButton::middlePressed) { 
    //middle button press to move to next color
    changeARGBColor(ARGBChannel::updateValueMethods::cycleValue, ARGBChannel::cycleValueDirection::increaseValue);
  }
  else if (be == PushButton::longPressed) { 
    //long button press to move to next channel
    changeARGBChannel(ARGBChannel::cycleValueDirection::increaseValue);
  }

  //listen for IR receiver command
  ARGBControllerIRParser::IRCommandEvent ice = argbIRReceiver.parseIRCommand();

  if (ice.changeMode) {
    //ir receiver change mode event
    changeARGBMode(ARGBChannel::updateValueMethods::cycleValue, ice.modeUpOrDown);
  }
  else if (ice.changeColor) {
    //ir receiver change color event
    changeARGBColor(ARGBChannel::updateValueMethods::assignValue, ice.colorIndex);
  }
  else if (ice.changeChannel) {
    //ir receiver change channel event
    changeARGBChannel(ice.channelUpOrDown);
  }

  //run all argb modes passing the IR receiver idle state param, with their respective timers inside
  //when IR receiver is not idle, don't strip.show() for all channels
  channel1.runARGBMode(argbIRReceiver.isIdle());
  channel2.runARGBMode(argbIRReceiver.isIdle());
  channel3.runARGBMode(argbIRReceiver.isIdle());
  channel4.runARGBMode(argbIRReceiver.isIdle());
  channel5.runARGBMode(argbIRReceiver.isIdle());
  channel6.runARGBMode(argbIRReceiver.isIdle());
  channel7.runARGBMode(argbIRReceiver.isIdle());
  channel8.runARGBMode(argbIRReceiver.isIdle());
}
