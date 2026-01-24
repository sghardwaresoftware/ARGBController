/*
ARGB modes
1. static red
2. static orange
3. static yellow
4. static green
5. static dark blue
6. argb off
7. color wipe red
8. color wipe yellow
9. color wipe green
10. color wipe dark blue
11. color chase red
12. color chase yellow
13. color chase green
14. color chase dark blue
15. rainbow cycle
16. rainbow waterfall
*/

#pragma once

#ifndef ARGBCHANNEL_H
#define ARGBCHANNEL_H

#include <Adafruit_NeoPixel.h>

class ARGBChannel {
public: 
  enum ARGBChannelConstants : uint8_t {numberOfModes = 7, numberOfColors = 7};
  enum ARGBColors : uint32_t {
    BLACK = 0x000000,   	//index 0
    RED = 0xFF0000,     	//index 1
    ORANGE = 0xFF6000,  	//index 2
    YELLOW = 0xFFFF00,  	//index 3
    GREEN = 0x00FF00,   	//index 4
    DARKBLUE = 0x0000FF,    //index 5
    MAGENTA = 0xFF00FF,  	//index 6
	WHITE = 0xFFFFFF		//index 7
  };

private:
  Adafruit_NeoPixel strip;
  uint8_t LEDCount;
  uint8_t ARGBMode;						//ARGB mode integer
  uint32_t ARGBColor;					//32 bit color RGB value
  
  uint8_t pixelInterval;                //Pixel Interval (ms); defines speed of argb pattern
  unsigned long prevPixelTime = 0;
  
  uint8_t pixelIndex = 0;
  bool colorWipeInverse = 0;
  uint8_t marqueeLoopCount = 0;
  uint8_t marqueePhase = 0;
  uint8_t rainbowCycle = 0;

public:
	//constructor to create neopixel object, with the given digital pin and LED count
	ARGBChannel(uint8_t digitalPin, uint8_t LEDCount) : 
	strip(LEDCount, digitalPin, NEO_GRB + NEO_KHZ800), LEDCount(LEDCount) {}

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
			ARGBColor = DARKBLUE;
			break;
		  case 6:
			ARGBColor = MAGENTA;
			break;
		  case 7:
			ARGBColor = WHITE;
			break;
		}
	}
  
	void begin(uint8_t initMode, uint8_t initPixelInterval, uint8_t brightness) {
		//initialize channel strip mode, color index and pixel interval based on given params
		ARGBMode = initMode;
		pixelInterval = initPixelInterval;
		
		//initalize the neopixel object
		strip.begin();
		strip.show();
		strip.setBrightness(brightness); // Set argb brightness (max = 255)
	}

	void updateARGBMode(uint8_t updateValue) {
		//update the ARGB mode with new mode number 
		ARGBMode = updateValue;
		
		//reset the channel's ARGB strip
		strip.fill(BLACK); strip.show();
		delay(25);
	}

	void runARGBMode() {
		unsigned long pixelTime = millis();
		
		//every pixel interval, execute the argb mode and compute new pixels
		if (pixelTime - prevPixelTime >= pixelInterval) {
		  prevPixelTime = pixelTime;

		  switch (ARGBMode) {
			case 1:
			  staticColor(RED); break;
			case 2:
			  staticColor(ORANGE); break;
			case 3:
			  staticColor(YELLOW); break;
			case 4:
			  staticColor(GREEN); break;
			case 5:
			  staticColor(DARKBLUE); break;
			case 6:
			  staticColor(BLACK); break;
			case 7:
			  colorWipe(RED, 50); break;
			case 8:
			  colorWipe(YELLOW, 50); break;
			case 9:
			  colorWipe(GREEN, 50); break;
			case 10:
			  colorWipe(DARKBLUE, 50); break;
			case 11:
			  colorChase(RED, 50); break;
			case 12:
			  colorChase(YELLOW, 50); break;
			case 13:
			  colorChase(GREEN, 50); break;
			case 14:
			  colorChase(DARKBLUE, 50); break;
			case 15:
			  rainbowCycling(100); break;
			case 16:
			  rainbowWaterfall(70); break;
		  }
		}
	}

	//BELOW ARE ALL THE RESPECTIVE ARGB MODE FUNCTIONS

	//static color mode: lights up the whole strip with one color and remains on
	void staticColor(uint32_t color) {
		strip.fill(color);
		strip.show();
	}

	// single color colorWipe
	// Fill strip pixels one after another, then alternates to turn off one by one
	void colorWipe(uint32_t color, int wait) {
	  pixelInterval = wait;                        //  Update delay time
	  
	  if (!colorWipeInverse) {
		  strip.setPixelColor(pixelIndex, color); //  Set pixel's color (in RAM)
	  }
	  else {
		  strip.setPixelColor(pixelIndex, strip.Color(0, 0, 0)); //  Set pixel's color (in RAM)
	  }
	  
	  strip.show(); 
	  pixelIndex++;
	  
	  if(pixelIndex >= LEDCount) {  
		pixelIndex = 0;
		colorWipeInverse ^= 1;
	  }
	}

	// 2-color colorWipe: alternating colorA and B after every full wipe
	void biColorWipe(uint32_t colorA, uint32_t colorB, uint8_t wait) {
	  pixelInterval = wait;

	  if (!colorWipeInverse) {
		  strip.setPixelColor(pixelIndex, colorA); //  Set pixel's color (in RAM)
	  }
	  else {
		  strip.setPixelColor(pixelIndex, strip.Color(0, 0, 0)); //  Set pixel's colorA (in RAM)
		  for (int otherIndex = pixelIndex+1; otherIndex < LEDCount; otherIndex++) {
			  strip.setPixelColor(otherIndex, colorB); //  Set pixel's colorB (in RAM)
		  }
	  }
	  
	  strip.show(); 
	  pixelIndex++;
	  
	  if(pixelIndex >= LEDCount) {  
		pixelIndex = 0;
		colorWipeInverse ^= 1;
	  }
	}

	// Knight-rider style color chase; 2 pixels light up and run through continuously
	void colorChase(uint32_t color, uint8_t wait) {
		pixelInterval = wait;
		strip.clear();
		
		for (int otherIndex = pixelIndex; otherIndex < pixelIndex+2; otherIndex++) {
			if (otherIndex <= LEDCount) {
				strip.setPixelColor(otherIndex, color);
			}
			else { break; }
		}
		
		strip.show(); 
		pixelIndex += 1;
		
		if(pixelIndex >= LEDCount) {
			pixelIndex = 0;
		}
	}

	// Marquee chasing lights. Pixels spaced apart equally
	// and chase one another
	void theaterChase(uint32_t color, int wait) {
	  pixelInterval = wait;                   //  Update delay time

	  strip.clear();

	  for(int c = pixelIndex; c < LEDCount; c += 3) {
		strip.setPixelColor(c, color);
	  }
	  strip.show();

	  pixelIndex++;
	  if (pixelIndex >= 3) {
		pixelIndex = 0;
		marqueeLoopCount++;
	  }

	  if (marqueeLoopCount >= 10) {
		pixelIndex = 0;
		marqueeLoopCount = 0;
	  }
	}

	// Marquee chase with 2 colors spaced apart and shifting over time.
	void biColorTheaterChase(uint32_t c1, uint32_t c2, uint8_t wait) {
	  pixelInterval = wait;

	  strip.clear();

	  for (uint8_t i = marqueePhase; i < LEDCount; i += 4) {
		strip.setPixelColor(i, c1);
		strip.setPixelColor(i+2, c2);
	  }

	  strip.show();

	  marqueePhase++;
	  if (marqueePhase >= 3) {
		marqueePhase = 0;
		marqueeLoopCount++;
	  }

	  // optional completion behavior
	  if (marqueeLoopCount >= 10) {
		marqueePhase = 0;
		marqueeLoopCount = 0;
	  }
	}

	// Rainbow cycle along whole strip. Pass delay time (in ms) between frames.
	void rainbowCycling(uint8_t wait) {
	  pixelInterval = wait;      
	  
	  for(uint16_t i=0; i < LEDCount; i++) {
		strip.setPixelColor(i, Wheel((i + rainbowCycle) & 255)); //  Update delay time  
	  }
	  
	  strip.show();                                  //  Update strip to match
	  
	  rainbowCycle += 5;                             //  Advance current cycle
	  if(rainbowCycle >= 256) rainbowCycle = 0;      //  Loop the cycle back to the begining
	}

	// Rainbow waterfall: shift pixels along the strip and inject new rainbow at the start.
	void rainbowWaterfall(uint8_t wait) {
	  pixelInterval = wait;

	  // shift everything "down" one pixel: [0] -> [1] -> [2] ...
	  for (int i = LEDCount - 1; i > 0; i--) {
		strip.setPixelColor(i, strip.getPixelColor(i - 1));
	  }

	  // inject new color at the head
	  strip.setPixelColor(0, Wheel(rainbowCycle & 255));

	  strip.show();

	  rainbowCycle += 20;
	  if (rainbowCycle >= 256) rainbowCycle = 0;
	}

	// Input a value 0 to 255 to get a color value.
	// The colours are a transition r - g - b - back to r.
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
  
};

#endif