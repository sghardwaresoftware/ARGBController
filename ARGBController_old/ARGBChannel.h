#pragma once

#ifndef ARGBCHANNEL_H
#define ARGBCHANNEL_H

#include <Adafruit_NeoPixel.h>

class ARGBChannel {
public: 
  enum ARGBChannelConstants : uint8_t {numberOfModes = 7, numberOfColors = 7};
  enum ARGBColors : uint32_t {
    BLACK = 0x000000,   //index 0
    RED = 0xFF0000,     //index 1
    ORANGE = 0xFF6000,  //index 2
    YELLOW = 0xFFFF00,  //index 3
    GREEN = 0x00FF00,   //index 4
    CYAN = 0x00FFFF,    //index 5
    WHITE = 0xFFFFFF,   //index 6
    MAGENTA = 0xFF00FF  //index 7
  };
  enum updateValueMethods : uint8_t {cycleValue, assignValue};
  enum cycleValueDirection : uint8_t {increaseValue, decreaseValue};

private:
  Adafruit_NeoPixel strip;
  uint8_t LED_count;
  uint8_t ARGBMode;
  uint8_t ARGBColorIndex;
  uint32_t ARGBColor;
  uint8_t pixelInterval;                //time interval between strip pixel update in ms
  const uint8_t showStripInterval = 40; //time interval between strip show() call in ms
  unsigned long prevPixelTime = 0;
  unsigned long prevShowStripTime = 0;

  //breathing mode values
  uint8_t brightness = 0;               //led brightness
  uint8_t upOrDown = 0;                 //brightness up or down; 0 is up, 1 is down

  //colorWipe mode values
  uint8_t colorWipePixel = 0;

  //colorMarquee mode values
  uint8_t marqueeLoopCount = 0;
  uint8_t colorMarqueePixel = 0;

  //rainbow mode and rainbow marquee mode values
  uint8_t rainbowPixelCycle = 0;
  uint8_t rainbowPixelQueue = 0;

public:
  //constructor to create neopixel object, with the given digital pin and LED count
  ARGBChannel(uint8_t digitalPin, uint8_t LED_count) : 
  strip(LED_count, digitalPin, NEO_GRB + NEO_KHZ800), LED_count(LED_count) {}

  void setARGBColor(int colorIndex) {
    //function to convert colorIndex to actual ARGBColor
    //index reference is in ARGBColors enum
    switch (colorIndex) {
      case 1:
        ARGBColor = RED;
        break;
      case 2:
        ARGBColor = ORANGE;
        break;
      case 3:
        ARGBColor = YELLOW;
        break;
      case 4:
        ARGBColor = GREEN;
        break;
      case 5:
        ARGBColor = CYAN;
        break;
      case 6:
        ARGBColor = WHITE;
        break;
      case 7:
        ARGBColor = MAGENTA;
        break;
    }
  }
  
  void begin(uint8_t initMode, uint8_t initColorIndex, uint8_t initPixelInterval) {
    //initialize channel strip mode, color index and pixel interval based on given params
    ARGBMode = initMode;
    ARGBColorIndex = initColorIndex;
    pixelInterval = initPixelInterval;
    
    //assign the ARGB color from the color index
    setARGBColor(ARGBColorIndex);
    
    //initalize the neopixel object
    strip.begin();
    strip.show();
  }

  uint8_t updateARGBMode(uint8_t updateValueMethod, uint8_t updateValue) {
    //update the ARGB mode with specified method

    if (updateValueMethod == cycleValue) {
      //update the ARGB mode by increasing/decreasing value
      if (updateValue == increaseValue) {
        if (ARGBMode == numberOfModes) { ARGBMode = 1; }
        else { ARGBMode += 1; }
      }
      else if (updateValue == decreaseValue) {
        if (ARGBMode == 1) { ARGBMode = numberOfModes; }
        else { ARGBMode -= 1; }
      }
    }
    else if (updateValueMethod == assignValue) {
      //update the ARGB mode by directly setting value
      ARGBMode = updateValue;
    }
    
    //reset the channel's ARGB strip
    strip.fill(BLACK); strip.show();
    delay(25);
    return ARGBMode;
  }

  uint8_t updateARGBColor(uint8_t updateValueMethod, uint8_t updateValue) {
    //update the ARGB color with specified method

    if (updateValueMethod == cycleValue) {
      if (updateValue == increaseValue) {
        if (ARGBColorIndex == numberOfColors) { ARGBColorIndex = 1; }
        else { ARGBColorIndex += 1; }
      }
      else if (updateValue == decreaseValue) {
        if (ARGBColorIndex == 1) { ARGBColorIndex = numberOfColors; }
        else { ARGBColorIndex -= 1; }
      }
    }
    else if (updateValueMethod == assignValue) {
      //update the ARGB color by directly setting value
      ARGBColorIndex = updateValue;
    }
    
    setARGBColor(ARGBColorIndex); //update the new ARGB color
    //reset the channel's ARGB strip
    strip.fill(BLACK); strip.show();
    delay(25);

    return ARGBColorIndex;
  }

  void runARGBMode(bool isIRReceiverIdle) {
    unsigned long pixelTime = millis();
    unsigned long showStripTime = millis();
    
    //every pixel interval, execute the argb mode and compute new pixels
    if (pixelTime - prevPixelTime >= pixelInterval) {
      prevPixelTime = pixelTime;

      switch (ARGBMode) {
        case 1:
          staticMode();
          break;
        case 2:
          breathingMode();
          break;
        case 3:
          colorWipeMode();
          break;
        case 4:
          colorMarqueeMode();
          break;
        case 5:
          rainbowMode();
          break;
        case 6:
          rainbowMarqueeMode();
          break;
        case 7:
          lightsOffMode();
          break;
      }
    }

    //every show argb strip interval, call strip.show() to refresh the argb strip
    //only if IR receiver is idle. Otherwise, don't execute strip.show()
    if (showStripTime - prevShowStripTime >= showStripInterval && isIRReceiverIdle) {
      prevShowStripTime = showStripTime;
      strip.show();
    }
  }

  //BELOW ARE ALL THE RESPECTIVE ARGB MODE FUNCTIONS

  //mode 1: static
  void staticMode() {
    strip.fill(ARGBColor);
    strip.setBrightness(192);     //set brightness to about 75% (max = 255)
    //strip.show();
  }

  //mode 2: breathing
  void breathingMode() {
    if (upOrDown == 0) {brightness += 15;}
    else {brightness -= 15;}
    
    //if brightness is max or min, inverse directions
    if (brightness >= 255 && upOrDown == 0) {upOrDown = 1;}
    else if (brightness <= 0 && upOrDown == 1) {upOrDown = 0;}

    strip.fill(ARGBColor);
    strip.setBrightness(brightness);
    //strip.show();
  }

  //mode 3: color wipe
  void colorWipeMode() {
    strip.setPixelColor(colorWipePixel, ARGBColor);
    strip.setBrightness(192);     //set brightness to about 75% (max = 255)
    //strip.show();
    colorWipePixel++;
    
    if (colorWipePixel >= LED_count) {
      colorWipePixel = 0;
      strip.clear();
    }
  }

  //mode 4: color marquee chase
  void colorMarqueeMode() {
    strip.clear();

    for (uint8_t c = colorMarqueePixel; c < LED_count; c += 3) {
      strip.setPixelColor(c, ARGBColor);
    }
    strip.setBrightness(192);     //set brightness to about 75% (max = 255)
    //strip.show();
    colorMarqueePixel++;

    if (colorMarqueePixel >= 3) {
      colorMarqueePixel = 0;
      marqueeLoopCount++;
    }
  
    if (marqueeLoopCount >= 10) {
      colorMarqueePixel = 0;
      marqueeLoopCount = 0;
    }
  }

  // helper wheel function for rainbowMode and rainbowMarqueeMode to transit r - g - b - back to r.
  uint32_t Wheel(byte WheelPos) {
    WheelPos = 255 - WheelPos;
    if(WheelPos < 85) {
      return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
    }
    if(WheelPos < 170) {
      WheelPos -= 85;
      return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
    }
    WheelPos -= 170;
    return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  }

  //mode 5: rainbow rgb cycle
  void rainbowMode() {
    for (uint16_t i = 0; i < LED_count; i++) {
       strip.setPixelColor(i, Wheel((i + rainbowPixelCycle) & 255));
    }
    strip.setBrightness(192);     //set brightness to about 75% (max = 255)
    //strip.show();
    rainbowPixelCycle++;

    if(rainbowPixelCycle >= 256) { rainbowPixelCycle = 0; }
  }

  //mode 6: rainbow rgb cycle with marquee chase; combination of modes 4 and 5
  void rainbowMarqueeMode() {
    strip.clear();

    for (uint8_t c = colorMarqueePixel; c < LED_count; c += 3) {
      strip.setPixelColor(c, Wheel((c + rainbowPixelCycle) & 255));
    }
    strip.setBrightness(192);     //set brightness to about 75% (max = 255)
    //strip.show();
    colorMarqueePixel++; rainbowPixelCycle++;

    if(rainbowPixelCycle >= 256) { rainbowPixelCycle = 0; }

    if (colorMarqueePixel >= 3) {
      colorMarqueePixel = 0;
      marqueeLoopCount++;
    }
  
    if (marqueeLoopCount >= 10) {
      colorMarqueePixel = 0;
      marqueeLoopCount = 0;
    }
  }

  //mode 7: lights turned off
  void lightsOffMode() {
    strip.fill(BLACK);
    //strip.show();
  }
  
};

#endif
