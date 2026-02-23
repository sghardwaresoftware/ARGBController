#include <Adafruit_NeoPixel.h>

#define LED_PIN 6

// How many NeoPixels are attached to the Arduino?
#define LED_COUNT 15

// Declare our NeoPixel strip object:
Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

int           pixelInterval = 0;        // Pixel Interval (ms); defines speed of argb pattern
unsigned long pixelPrevious = 0;        // Previous Pixel Millis

unsigned long patternPrevious = 0;      // Previous Pattern Millis
int           patternCurrent = 1;       // Current Pattern Number
int           patternInterval = 5000;   // Pattern Interval (ms)

int			  pixelIndex = 0;
uint16_t      pixelNumber = LED_COUNT;  // Total Number of Pixels

bool colorWipeInverse = 0;
int marqueeLoopCount = 0;
int marqueePhase = 0;
int rainbowCycle = 0;

void setup() {
  strip.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
  strip.show();            // Turn OFF all pixels ASAP
  strip.setBrightness(192); // Set BRIGHTNESS to about 1/5 (max = 255)
}

void loop() {
  unsigned long currentMillis = millis();                     //  Update current time
  if((currentMillis - patternPrevious) >= patternInterval) {  //  Check for expired time
    patternPrevious = currentMillis;
    patternCurrent++;                                         //  Advance to next pattern
    if(patternCurrent > 6)
      patternCurrent = 1;
  }

  if(currentMillis - pixelPrevious >= pixelInterval) {        //  Check for expired time
    pixelPrevious = currentMillis;                            //  Run current frame
    switch (patternCurrent) {
      case 1:
        colorWipe(strip.Color(255, 0, 0), 50);
        break;
      case 2:
        biColorWipe(strip.Color(0, 255, 0), strip.Color(0, 0, 255), 50);
        break;
      case 3:
        colorChase(strip.Color(255, 255, 0), 50);                                                                                                                                                                                                                                                                                                                                                                                                                                                                                  
        break;        
      case 4:
        biColorTheaterChase(strip.Color(255, 255, 255), strip.Color(255, 0, 255), 75); 
        break;
      case 5:
        rainbowCycling(100); // Flowing rainbow cycle along the whole strip
        break;   
      case 6:
        rainbowWaterfall(70); // Flowing rainbow cycle along the whole strip
        break;   
      default:
        break;
    }
  }
}

// Some functions of our own for creating animated effects -----------------

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
  
  if(pixelIndex >= pixelNumber) {  
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
      for (int otherIndex = pixelIndex+1; otherIndex < pixelNumber; otherIndex++) {
		  strip.setPixelColor(otherIndex, colorB); //  Set pixel's colorB (in RAM)
	  }
  }
  
  strip.show(); 
  pixelIndex++;
  
  if(pixelIndex >= pixelNumber) {  
    pixelIndex = 0;
    colorWipeInverse ^= 1;
  }
}

// Knight-rider style color chase; 2 pixels light up and run through continuously
void colorChase(uint32_t color, uint8_t wait) {
	pixelInterval = wait;
	strip.clear();
	
	for (int otherIndex = pixelIndex; otherIndex < pixelIndex+2; otherIndex++) {
		if (otherIndex <= pixelNumber) {
			strip.setPixelColor(otherIndex, color);
		}
		else { break; }
	}
	
	strip.show(); 
	pixelIndex += 1;
	
	if(pixelIndex >= pixelNumber) {
		pixelIndex = 0;
	}
}

// Theater-marquee-style chasing lights. Pixels spaced apart equally
// and chase one another
void theaterChase(uint32_t color, int wait) {
  pixelInterval = wait;                   //  Update delay time

  strip.clear();

  for(int c = pixelIndex; c < pixelNumber; c += 3) {
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

  for (uint8_t i = marqueePhase; i < pixelNumber; i += 4) {
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
  
  for(uint16_t i=0; i < pixelNumber; i++) {
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
  for (int i = pixelNumber - 1; i > 0; i--) {
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
