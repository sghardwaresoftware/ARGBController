// A non-blocking everyday NeoPixel strip test program.

// NEOPIXEL BEST PRACTICES for most reliable operation:
// - Add 1000 uF CAPACITOR between NeoPixel strip's + and - connections.
// - MINIMIZE WIRING LENGTH between microcontroller board and first pixel.
// - NeoPixel strip's DATA-IN should pass through a 300-500 OHM RESISTOR.
// - AVOID connecting NeoPixels on a LIVE CIRCUIT. If you must, ALWAYS
//   connect GROUND (-) first, then +, then data.
// - When using a 3.3V microcontroller with a 5V-powered NeoPixel strip,
//   a LOGIC-LEVEL CONVERTER on the data line is STRONGLY RECOMMENDED.
// (Skipping these may work OK on your workbench but can fail in the field)

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
 #include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

#define LED_PIN 5

// How many NeoPixels are attached to the Arduino?
#define LED_COUNT 8

// Declare our NeoPixel strip object:
Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);
// Argument 1 = Number of pixels in NeoPixel strip
// Argument 2 = Arduino pin number (most are valid)
// Argument 3 = Pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//   NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)

unsigned long pixelPrevious = 0;        // Previous Pixel Millis
unsigned long patternPrevious = 0;      // Previous Pattern Millis
int           patternCurrent = 1;       // Current Pattern Number
int           patternInterval = 5000;   // Pattern Interval (ms)
bool          patternComplete = false;

int           pixelInterval = 50;       // Pixel Interval (ms)
int           pixelQueue = 0;           // Pattern Pixel Queue
int           pixelCycle = 0;           // Pattern Pixel Cycle
uint16_t      pixelNumber = LED_COUNT;  // Total Number of Pixels

// setup() function -- runs once at startup --------------------------------
void setup() {
  // These lines are specifically to support the Adafruit Trinket 5V 16 MHz.
  // Any other board, you can remove this part (but no harm leaving it):
#if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
  clock_prescale_set(clock_div_1);
#endif
  // END of Trinket-specific code.

  strip.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
  strip.show();            // Turn OFF all pixels ASAP
  strip.setBrightness(160); // Set BRIGHTNESS to about 1/5 (max = 255)
}

// loop() function -- runs repeatedly as long as board is on ---------------
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
        colorWipe(strip.Color(255, 0, 0), 75);
        break;
      case 2:
        biColorWipe(strip.Color(0, 255, 0), strip.Color(0, 0, 255), 75);
        break;
      case 3:
        theaterChase3Colors(strip.Color(128,255,0), strip.Color(255,255,0), strip.Color(0,0,255), 50);
        break;        
      case 4:
        theaterChase(strip.Color(127, 0, 0), 50); // Red
        break;
      case 5:
        rainbow(100); // Flowing rainbow cycle along the whole strip
        break;   
      case 6:
        rainbowWaterfall(50); // Flowing rainbow cycle along the whole strip
        break;   
      default:
        break;
    }
  }
}

// Some functions of our own for creating animated effects -----------------

// single color colorWipe
// Fill strip pixels one after another with a color. Anything there will be covered pixel by pixel. 
void colorWipe(uint32_t color, int wait) {
  static uint16_t current_pixel = 0;
  pixelInterval = wait;                        //  Update delay time
  strip.setPixelColor(current_pixel++, color); //  Set pixel's color (in RAM)
  strip.show();                                //  Update strip to match
  if(current_pixel >= pixelNumber) {           //  Loop the pattern from the first LED
    current_pixel = 0;
    strip.clear();
    strip.show();
  }
}

// 2-color colorWipe: first half uses colorA, second half uses colorB.
// After reaching the end, it clears and starts again with colorA on the first half.
void biColorWipe(uint32_t colorA, uint32_t colorB, uint8_t wait) {
  static uint16_t idx = 0;     // which pixel we're currently writing
  static bool secondHalf = false;

  pixelInterval = wait;

  uint16_t half = pixelNumber / 2;  // midpoint (for odd counts, second half is slightly larger)

  // Pick which color to write based on whether we are in the first or second half
  uint32_t c = (idx < half) ? colorA : colorB;

  strip.setPixelColor(idx, c);
  strip.show();

  idx++;

  // When we hit the end, reset for next cycle
  if (idx >= pixelNumber) {
    idx = 0;
    secondHalf = false;   // (not strictly needed, kept for clarity)
    strip.clear();
    strip.show();
  }
}

// Theater-marquee-style chasing lights. Pass in a color (32-bit value,
// a la strip.Color(r,g,b) as mentioned above), and a delay time (in ms)
// between frames.
void theaterChase(uint32_t color, int wait) {
  static uint32_t loop_count = 0;
  static uint16_t current_pixel = 0;

  pixelInterval = wait;                   //  Update delay time

  strip.clear();

  for(int c=current_pixel; c < pixelNumber; c += 3) {
    strip.setPixelColor(c, color);
  }
  strip.show();

  current_pixel++;
  if (current_pixel >= 3) {
    current_pixel = 0;
    loop_count++;
  }

  if (loop_count >= 10) {
    current_pixel = 0;
    loop_count = 0;
  }
}

// Marquee chase with 3 colors interleaved (all present at once) and shifting over time.
void theaterChase3Colors(uint32_t c1, uint32_t c2, uint32_t c3, uint8_t wait) {
  static uint32_t loop_count = 0;
  static uint8_t phase = 0; // 0..2

  pixelInterval = wait;

  strip.clear();

  for (uint16_t i = 0; i < pixelNumber; i++) {
    uint8_t idx = (i + phase) % 3;
    if (idx == 0)      strip.setPixelColor(i, c1);
    else if (idx == 1) strip.setPixelColor(i, c2);
    else               strip.setPixelColor(i, c3);
  }

  strip.show();

  phase++;
  if (phase >= 3) {
    phase = 0;
    loop_count++;
  }

  // optional completion behavior
  if (loop_count >= 10) {
    phase = 0;
    loop_count = 0;
  }
}

// Rainbow cycle along whole strip. Pass delay time (in ms) between frames.
void rainbow(uint8_t wait) {
  if(pixelInterval != wait)
    pixelInterval = wait;                   
  for(uint16_t i=0; i < pixelNumber; i++) {
    strip.setPixelColor(i, Wheel((i + pixelCycle) & 255)); //  Update delay time  
  }
  strip.show();                             //  Update strip to match
  pixelCycle+=5;                             //  Advance current cycle
  if(pixelCycle >= 256)
    pixelCycle = 0;                         //  Loop the cycle back to the begining
}

// Rainbow waterfall: shift pixels along the strip and inject new rainbow at the start.
void rainbowWaterfall(uint8_t wait) {
  pixelInterval = wait;

  // shift everything "down" one pixel: [0] -> [1] -> [2] ...
  for (int i = pixelNumber - 1; i > 0; i--) {
    strip.setPixelColor(i, strip.getPixelColor(i - 1));
  }

  // inject new color at the head
  strip.setPixelColor(0, Wheel(pixelCycle & 255));

  strip.show();

  pixelCycle+=30;
  if (pixelCycle >= 256) pixelCycle = 0;
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
