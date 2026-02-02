#include "./PushButton.h"
#include "./ChannelsLEDs.h"
#include "./ARGBControllerMem.h"
#include "./SoftSerialBytes.h"

#define numberOfChannels 8
#define numberOfModes 16

#define serialTxFlag_pin A0
/*
 * shift register to send out bits for channel active indicator
 * only one of the 8 bits is '1' to indicate that channel is active
 */  
#define channelLED_shclk_pin 2
#define channelLED_stclk_pin 3
#define channelLED_data_pin 4

#define argb_button_pin A5

SoftSerialBytes softSerial(9600);
ARGBControllerMem ARGBMemory;
PushButton argbButton(argb_button_pin);
ChannelsLEDs channelLEDs(channelLED_shclk_pin, channelLED_stclk_pin, channelLED_data_pin);

uint8_t currentChannel = 1;
uint8_t channelModes[numberOfChannels] = {0, 0, 0, 0, 0, 0, 0, 0};

byte createSerialByte(uint8_t channel, uint8_t mode) {
  return (channel << 4) | mode;
}

void initARGBValues() {
  ARGBControllerMem::ARGBInitValues initValues = ARGBMemory.begin();
  digitalWrite(serialTxFlag_pin, HIGH);
  
  for (uint8_t i; i < numberOfChannels; i++) {
    channelModes[i] = initValues.modeValues[i];
    softSerial.sendByte( createSerialByte(i+1, channelModes[i]) );
  }
  
  digitalWrite(serialTxFlag_pin, LOW);
}

void setup() {
  pinMode(serialTxFlag_pin, OUTPUT); digitalWrite(serialTxFlag_pin, LOW);
  argbButton.begin();
  channelLEDs.begin(currentChannel);
  softSerial.begin();
  delay(1500);
  initARGBValues();
}

void loop() {
  //read argb button for short press (change mode), middle press (change channel)
  PushButton::ButtonEvent be = argbButton.listenButton(); 

  if (be == PushButton::shortPressed) { 
    //short button press to move to next mode
    channelModes[currentChannel - 1] += 1;
    ARGBMemory.updateMemory(currentChannel - 1, channelModes[currentChannel - 1]);
    
    digitalWrite(serialTxFlag_pin, HIGH);
    softSerial.sendByte( createSerialByte(currentChannel, channelModes[currentChannel - 1]) );
    digitalWrite(serialTxFlag_pin, LOW);
    
    if (channelModes[currentChannel - 1] > numberOfModes) { 
      channelModes[currentChannel - 1] = 1; 
    }
  }
  else if (be == PushButton::middlePressed) { 
    //middle button press to move to next channel
    currentChannel ++;
    if (currentChannel > numberOfChannels) { currentChannel = 1; }
  }
}
