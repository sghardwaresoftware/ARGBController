#include "./PushButton.h"
#include "./ChannelsLEDs.h"

#define numberOfChannels 8
#define numberOfModes 16
#define setChannelModeDelay 1000 //delay in setting the channel & mode bits in ms

/*
 * shift register to send out bits for channel and mode to MCU 1
 * bit 7: 0, bit 6: ch_bit2, bit5: ch_bit1, bit4: ch_bit0
 * bit 3: mode_bit3, bit 2: mode_bit2, bit 1: mode_bit1, bit 0: mode_bit0,
 */  
#define channelMode_shclk_pin 2
#define channelMode_stclk_pin 3
#define channelMode_data_pin 4
#define channelModeChangeFlag_pin 5

/*
 * shift register to send out bits for channel active indicator
 * only one of the 8 bits is '1' to indicate that channel is active
 */  
#define channelLED_shclk_pin 6
#define channelLED_stclk_pin 7
#define channelLED_data_pin 8

#define argb_button_pin A5

PushButton argbButton(argb_button_pin);
ChannelsLEDs channelLEDs(channelLED_shclk_pin, channelLED_stclk_pin, channelLED_data_pin);

uint8_t currentChannel = 1;
uint8_t currentMode = 1;

void resetChannelMode() {
  //set all channel and mode bits to 0
  digitalWrite(channelMode_stclk_pin, LOW);
  shiftOut(channelMode_data_pin, channelMode_shclk_pin, MSBFIRST, 0);
  digitalWrite(channelMode_stclk_pin, HIGH);
}

void setCurrentChannel() {
  digitalWrite(channelModeChangeFlag_pin, HIGH);
  digitalWrite(channelMode_stclk_pin, LOW);
  shiftOut(channelMode_data_pin, channelMode_shclk_pin, MSBFIRST, currentChannel << 4);
  digitalWrite(channelMode_stclk_pin, HIGH);
  delay(setChannelModeDelay);
  digitalWrite(channelModeChangeFlag_pin, LOW);
}

uint8_t setCurrentMode() {
  digitalWrite(channelModeChangeFlag_pin, HIGH);
  digitalWrite(channelMode_stclk_pin, LOW);
  shiftOut(channelMode_data_pin, channelMode_shclk_pin, MSBFIRST, currentMode);
  digitalWrite(channelMode_stclk_pin, HIGH);
  delay(setChannelModeDelay);
  digitalWrite(channelModeChangeFlag_pin, LOW);
}

void setup() {
  argbButton.begin();
  channelLEDs.begin(currentChannel);
  pinMode(channelMode_shclk_pin, OUTPUT); pinMode(channelMode_stclk_pin, OUTPUT); pinMode(channelMode_data_pin, OUTPUT); 
  pinMode(channelModeChangeFlag_pin, OUTPUT);
  resetChannelMode();
}

void loop() {
  //read argb button for short press (change mode), middle press (change channel)
  PushButton::ButtonEvent be = argbButton.listenButton(); 

  if (be == PushButton::shortPressed) { 
    //short button press to move to next mode
    currentMode ++;
    setCurrentMode();
    if (currentMode > numberOfModes) { currentMode = 1; }
  }
  else if (be == PushButton::middlePressed) { 
    //middle button press to move to next channel
    currentChannel ++;
    setCurrentChannel();
    if (currentChannel > numberOfChannels) { currentChannel = 1; }
  }
}
