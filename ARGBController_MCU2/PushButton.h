#pragma once
#ifndef PUSHBUTTON_H
#define PUSHBUTTON_H

#include <Arduino.h>

class PushButton {

//class public constants
public:
  enum PushButtonConstants : int {debounceDelay = 50, minShortPressDelay = 75, shortPressThreshold = 450, longPressThreshold = 1000, maxLongPressDelay = 2500};

//class private attributes
private:
  const int pushButtonPin;
  int buttonState;
  int prevButtonReading;
  unsigned long prevDebounceTime;
  unsigned long buttonPressTime; // keep the button press time to wait for the release time

//class public functions
public:
  enum ButtonEvent : int {notPressed = 0, shortPressed = 1, middlePressed = 2, longPressed = 3};

  PushButton(int pushButtonPin) : pushButtonPin(pushButtonPin) {
    prevButtonReading = HIGH;
    prevDebounceTime = 0;
    buttonPressTime = 0;
  }

  void begin() {
    pinMode(pushButtonPin, INPUT);
  }

  ButtonEvent listenButton() {
    int buttonReading = digitalRead(pushButtonPin);

    if (buttonReading != prevButtonReading) {
      prevDebounceTime = millis();
    }

    if (millis() - prevDebounceTime > debounceDelay) {
      //button debounce is over, check exact button state
      if (buttonReading != buttonState) {
        buttonState = buttonReading;

        if (buttonState == LOW) {
          //button is pressed
          buttonPressTime = millis();
        }
        else if (buttonState == HIGH) {
          //button is released
          unsigned long buttonReleaseTime = millis();
          unsigned long buttonDelay = buttonReleaseTime - buttonPressTime;
          
          //if button delay is between 75 and 450ms, its short pressed
          if (buttonDelay > minShortPressDelay && buttonDelay <= shortPressThreshold) { return shortPressed; }
          
          //else if button delay is between 450 and 1000ms, its middle pressed
          else if (buttonDelay > shortPressThreshold && buttonDelay <= longPressThreshold) { return middlePressed; }

          //else if button delay is between 1000 and 2500ms, its long pressed
          else if (buttonDelay > longPressThreshold && buttonDelay <= maxLongPressDelay) { return longPressed; }
        }
      }
      else { return notPressed; }
    }

    prevButtonReading = buttonReading;
  }
};

#endif
