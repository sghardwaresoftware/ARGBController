#include "./PushButton.h"
#include "./ChannelsLEDs.h"
#include "./ARGBControllerMem.h"
#include "./IRReceiver.h"
#include "./IRReceiverButtons.h"
#include "./SoftSerialBytes.h"

#define MCU_initDelay 1500
#define numberOfChannels 8
#define numberOfModes 16

/*
 * shift register to send out bits for channel active indicator
 * only one of the 8 bits is '1' to indicate that channel is active
 */  
#define channelLED_shclk_pin 2
#define channelLED_stclk_pin 3
#define channelLED_data_pin 4

#define argb_button_pin 5
#define irReceiver_pin 6

/*
 * softSerial uses pins 8 (RX) and 9 (TX)
 */
#define serialTxFlag_pin 10

IRReceiver irReceiver(irReceiver_pin);
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

void updateToARGBChannel() {
  Serial.print("Channel: "); Serial.print(currentChannel); 
  Serial.print("; Mode: "); Serial.println(channelModes[currentChannel], HEX);
  
  digitalWrite(serialTxFlag_pin, HIGH);
  softSerial.sendByte( createSerialByte(currentChannel, channelModes[currentChannel]) );
  digitalWrite(serialTxFlag_pin, LOW);
}

void nextARGBChannel() {
  currentChannel += 1;
  if (currentChannel == numberOfChannels) { currentChannel = 0; }
  channelLEDs.updateChannelsLEDs(currentChannel);
}

void prevARGBChannel() {
  currentChannel -= 1;
  if (currentChannel == 255) { currentChannel = 7; } // 0-1 in unsigned 8 bit overflows to 255
  channelLEDs.updateChannelsLEDs(currentChannel);
}

void nextARGBMode() {
  channelModes[currentChannel] += 1;
  if (channelModes[currentChannel] == numberOfModes) { 
    channelModes[currentChannel] = 0; 
  }
  
  ARGBMemory.updateMemory(currentChannel, channelModes[currentChannel]);
  updateToARGBChannel();
}

void prevARGBMode() {
  channelModes[currentChannel] -= 1;
  if (channelModes[currentChannel] == 255) { // 0-1 in unsigned 8 bit overflows to 255 
    channelModes[currentChannel] = 15; 
  }

  ARGBMemory.updateMemory(currentChannel, channelModes[currentChannel]);
  updateToARGBChannel();
}

void jumpToARGBMode(uint8_t newMode) {
  channelModes[currentChannel] = newMode;
  ARGBMemory.updateMemory(currentChannel, channelModes[currentChannel]);
  updateToARGBChannel();
}

void setup() {
  Serial.begin(9600);
  pinMode(serialTxFlag_pin, OUTPUT); digitalWrite(serialTxFlag_pin, LOW);
  argbButton.begin();
  channelLEDs.begin(currentChannel);
  irReceiver.begin();
  softSerial.begin();
  delay(MCU_initDelay);
  initARGBValues();
}

void loop() {
  //read argb button for short press (change mode), middle press (change channel)
  PushButton::ButtonEvent be = argbButton.listenButton(); 
  uint16_t irReceiverCommand = irReceiver.listenIRReceiver();

  if (be == PushButton::shortPressed) { 
    //short button press to move to next mode
    nextARGBMode();
  }
  else if (be == PushButton::middlePressed) { 
    //middle button press to move to next channel
    nextARGBChannel();
  }

  if (irReceiverCommand != 0xFFFF) {
    /*
     * IR command received; process each command.
     * IR remote button and command mappings are in IRReceiverButtons.h
     */
    //Serial.println(irReceiverCommand);
    if (irReceiverCommand == CH_MINUS_BTN) { prevARGBChannel(); }
    else if (irReceiverCommand == CH_PLUS_BTN) { nextARGBChannel(); }
    else if (irReceiverCommand == PREV_BTN) { prevARGBMode(); }
    else if (irReceiverCommand == NEXT_BTN) { nextARGBMode(); }

    else if (irReceiverCommand == NO_0_BTN) { jumpToARGBMode(5); }
    else if (irReceiverCommand == NO_1_BTN) { jumpToARGBMode(0); }
    else if (irReceiverCommand == NO_2_BTN) { jumpToARGBMode(3); }
    else if (irReceiverCommand == NO_3_BTN) { jumpToARGBMode(4); }
    else if (irReceiverCommand == NO_4_BTN) { jumpToARGBMode(7); }
    else if (irReceiverCommand == NO_5_BTN) { jumpToARGBMode(9); }
    else if (irReceiverCommand == NO_6_BTN) { jumpToARGBMode(11); }
    else if (irReceiverCommand == NO_7_BTN) { jumpToARGBMode(13); }
    else if (irReceiverCommand == NO_8_BTN) { jumpToARGBMode(14); }
    else if (irReceiverCommand == NO_9_BTN) { jumpToARGBMode(15); }
  }
}
