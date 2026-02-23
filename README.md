# ARGBController
A custom Arduino project for individual ARGB control up to 8 channels

## Intro
Most of the existing budget multi-port ARGB fan controllers are designed to be one-for-all sync. This project, however, enables each port to be controlled individually,
allowing users to set different ARGB modes & colors on different devices as they wish. Written in Arduino IDE-friendly C++, it's portable to many low-cost
MCUs i.e ATmega328p, making another potential competitive budget ARGB controller hub.

## Features
<ul>
  <li>MCU stores & manipulates ARGB channel modes individually</li>
  <li>Channel LED indication</li>
  <li>Persistent storage using EEPROM</li>
  <li>Built-in push button and IR remote control</li>
  <li>Pre-programmed 16 ARGB modes per channel</li>
  <li>Low BOM optimized (can run on internal 8Mhz clock for 8-bit ATmega MCUs)</li>
</ul>

## Why 2 MCUs?
A very common question haha. The main scope of this project was to use the ATmega328p MCUs because I have a ton of them to clear (I was unable to think of any unique,
marketable application using that low-spec, resource-constrained MCU, besides this project)

One very common issue with using those MCUs is that executing NeoPixel code, especially `.show()`, disables interrupts, 
which serial communication, IR, and many other basic functions rely on.

Thus, I came up with the idea of using one MCU only to handle the NeoPixels, while the other to store the channel and mode values while listening for IR and button events :)

Of course, feel free to merge the two MCU1, MCU2 sketches yourself if other, more powerful MCUs are used.
 
## Requirements
<ul>
  <li>Arduino IDE version 1.8.19 or > 2.x</li>
  <li>Appropriate boards installed i.e. Minicore board (for 8-bit ATmega MCUs)</li>
  <li>Adafruit NeoPixel library installed</li>
  <li><a href="https://github.com/Arduino-IRremote/Arduino-IRremote" target="blank">IRRemote library</a> installed</li>
</ul>

## Reference Hardware Design
For curious developers, I have designed a custom PCB to fulfil my project scope as a reference. The schematic will be uploaded soon. Feel free to design your own hardware!

## Todo
Some later plans to be considered:
<ul>
  <li>Software control via USB-serial</li>
  <li>Including more ARGB modes</li>
  <li>Expanding to 10 or more channels</li>
  <li>Refactoring for powerful MCUs i.e. RP2040</li>
</ul>

## Special Thanks
I would like to thank the Adafruit and IRRemote team for the continued maintenance of the libraries to make this project possible.
