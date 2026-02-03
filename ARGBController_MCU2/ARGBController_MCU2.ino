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

/*
 * channel and mode numbers in MCU 2 start from 0
 * channel and mode numbers in MCU 1 start from 1
 */
uint8_t currentChannel = 0;
uint8_t channelModes[numberOfChannels] = {0, 0, 0, 0, 0, 0, 0, 0};

byte createSerialByte(uint8_t channel, uint8_t mode) {
  return (channel << 4) | mode;
}

void initARGBValues() {
  ARGBControllerMem::ARGBInitValues initValues = ARGBMemory.begin();
  digitalWrite(serialTxFlag_pin, HIGH);
  
  for (uint8_t c; c < numberOfChannels; c++) {
    channelModes[c] = initValues.modeValues[c];
    softSerial.sendByte( createSerialByte(c, channelModes[c]) );
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
    channelModes[currentChannel] += 1;
    if (channelModes[currentChannel] == numberOfModes) { 
      channelModes[currentChannel] = 0; 
    }
    
    ARGBMemory.updateMemory(currentChannel, channelModes[currentChannel]);
    
    digitalWrite(serialTxFlag_pin, HIGH);
    softSerial.sendByte( createSerialByte(currentChannel, channelModes[currentChannel]) );
    digitalWrite(serialTxFlag_pin, LOW);
  }
  else if (be == PushButton::middlePressed) { 
    //middle button press to move to next channel
    currentChannel ++;
    if (currentChannel == numberOfChannels) { currentChannel = 0; }
  }
}
