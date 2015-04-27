/*
  Copyright (c) 2015 Duane Johnson
  License: MIT
  
  This shows a colorful LED light demo for Make Salt Lake. The
  LED strip is 15 lights across by 10 down, with each second
  line wrapping back to the beginning (e.g. each second line of
  LEDs is reversed).

  - shows "MAKE SALT LAKE" scroll across
  - shows a colorful SinWave demo
  - shows "MAKE SLC" image
  - shows several colorful wipe screens
  - loops back to beginning

  OctoWS2811 BasicTest.ino - Basic RGB LED Test
  http://www.pjrc.com/teensy/td_libs_OctoWS2811.html
  Copyright (c) 2013 Paul Stoffregen, PJRC.COM, LLC

  Required Connections
  --------------------
    pin 2:  LED Strip #1    OctoWS2811 drives 8 LED Strips.
    pin 14: LED strip #2    All 8 are the same length.
    pin 7:  LED strip #3
    pin 8:  LED strip #4    A 100 ohm resistor should used
    pin 6:  LED strip #5    between each Teensy pin and the
    pin 20: LED strip #6    wire to the LED strip, to minimize
    pin 21: LED strip #7    high frequency ringining & noise.
    pin 5:  LED strip #8
    pin 15 & 16 - Connect together, but do not use
    pin 4 - Do not use
    pin 3 - Do not use as PWM.  Normal use is ok.
*/

#include <OctoWS2811.h>
#include "MixWave.h"

const int ledsPerStrip = 30;

DMAMEM int displayMemory[ledsPerStrip*6];
int drawingMemory[ledsPerStrip*6];

const int config = WS2811_RGB | WS2811_800kHz;

OctoWS2811 leds(ledsPerStrip, displayMemory, drawingMemory, config);

// We only have 7 unique letters in "MAKE SALT LAKE"... easy peasy!
// Note: To generate these, I used roznerd's Excel spreadsheet:
//   http://www.instructables.com/id/LED-Scolling-Dot-Matrix-Font-Graphics-Generator-/
const unsigned char l_M[] = { 0xFF, 0xFF, 0x70, 0x38, 0x70, 0xFF, 0xFF, 0x00 };
const unsigned char l_A[] = { 0x0F, 0x3F, 0x74, 0xC4, 0x74, 0x3F, 0x0F, 0x00 };
const unsigned char l_K[] = { 0xFF, 0xFF, 0x18, 0x3C, 0x6E, 0xC7, 0x83, 0x00 };
const unsigned char l_E[] = { 0xFF, 0xFF, 0xDB, 0xDB, 0xDB, 0xC3, 0x83, 0x00 };
const unsigned char l_S[] = { 0x64, 0xF6, 0xD3, 0xDB, 0xCB, 0x6F, 0x26, 0x00 };
const unsigned char l_L[] = { 0xFF, 0xFF, 0x03, 0x03, 0x03, 0x03, 0x01, 0x00 };
const unsigned char l_T[] = { 0x80, 0xC0, 0xC0, 0xFF, 0xFF, 0xC0, 0xC0, 0x00 };

// Red, Green & Blue
#define R  0x110000
#define G  0x001100
#define B  0x000011

#define DR  0x050000
#define DG  0x000500
#define DB  0x000005

// Yellow & Purple
#define Y  0x111100
#define P  0x110011

#define DY  0x050500
#define DP  0x050005

// Black
#define K  0x000000

// White
#define W  0x111111

const int makeslc[] = {
  R, K, R, K, K, R, K, K, R, K, R, K, R, R, R,
  R, R, R, K, R, K, R, K, R, K, R, K, R, K, K,
  R, DR,R, K, R, R, R, K, R, R, K, K, R, R, K,
  R, K, R, K, R, K, R, K, R, K, R, K, R, K, K,
  R, K, R, K, R, K, R, K, R, K, R, K, R, R, R,
  
  K, K, K, B, B, K, B, K, K, K, K, B, B, K, K,
  K, K, B, K, K, K, B, K, K, K, B, K, K, K, K,
  K, K, K, B, K, K, B, K, K, K, B, K, K, K, K,
  K, K, K, K, B, K, B, K, K, K, B, K, K, K, K,
  K, K, B, B, K, K, B, B, B, K, K, B, B, K, K
};

int buffer[15*10];
int scroll_x = 16;
int x_make = 0, x_salt = 36, x_lake = 70;
int microsec = 2000;

MixWave mix;

void setup() {
  leds.begin();
  leds.show();

  mix.init();
}

void loop() {

  if (scroll_x >= -110) {
    // First show "MAKE SALT LAKE" scroll by
    clearImage(buffer);
    drawLetter(buffer, l_M, scroll_x + x_make + 0, 1, G);
    drawLetter(buffer, l_A, scroll_x + x_make + 8, 1, Y);
    drawLetter(buffer, l_K, scroll_x + x_make + 16, 1, R);
    drawLetter(buffer, l_E, scroll_x + x_make + 24, 1, B);
  
    drawLetter(buffer, l_S, scroll_x + x_salt + 0, 1, R);
    drawLetter(buffer, l_A, scroll_x + x_salt + 8, 1, R);
    drawLetter(buffer, l_L, scroll_x + x_salt + 16, 1, R);
    drawLetter(buffer, l_T, scroll_x + x_salt + 22, 1, R);
  
    drawLetter(buffer, l_L, scroll_x + x_lake + 0, 1, B);
    drawLetter(buffer, l_A, scroll_x + x_lake + 8, 1, B);
    drawLetter(buffer, l_K, scroll_x + x_lake + 16, 1, B);
    drawLetter(buffer, l_E, scroll_x + x_lake + 24, 1, B);
    showImage(buffer);
  
    delayMicroseconds(100000);
  
    scroll_x--;
  } else if (scroll_x >= -200) {
    // Then show the awesome SinWave mixer
    mix.draw(buffer);
    mix.step();
    showImage(buffer);
    
    scroll_x--;
  } else {
    // And finally, show the "MakeSLC" image,
    showImage((int *)makeslc);
    delayMicroseconds(2500000);

    // followed by some colorful wipe screens
    colorWipe(G, microsec);
    colorWipe(DG, microsec);

    colorWipe(Y, microsec);
    colorWipe(DY, microsec);

    colorWipe(R, microsec);
    colorWipe(DR, microsec);

    colorWipe(P, microsec);
    colorWipe(DP, microsec);

    colorWipe(B, microsec);
    colorWipe(DB, microsec);

    // Reset the scroll point for "MAKE SALT LAKE" back to the beginning
    scroll_x = 16;
  }
}

void colorWipe(int color, int wait)
{
  for (int i=0; i < leds.numPixels(); i++) {
    leds.setPixel(i, color);
    leds.show();
    delayMicroseconds(wait);
  }
}

int getIdx(int x, int y) {
  if (y % 2 == 1) {
    return y*15+(14-x);
  } else {
    return y*15+x;
  }
}

void showImage(int img[]) {
  int i = 0;
  for (int y = 0; y < 10; y++) {
    for (int x = 0; x < 15; x++) {
      leds.setPixel(getIdx(x, y), img[i++]);
    }
  }
  leds.show();
}

void clearImage(int img[]) {
  for (int i = 0; i < 10*15; i++) {
    img[i] = 0;
  }
}

void drawPixel(int img[], int x, int y, int color) {
  // clipping
  if (x >= 0 && x < 15 && y >= 0 && y < 10) {
    img[ y * 15 + x ] = color;
  }
}

void drawLetter(int img[], const unsigned char letter[], int x, int y, int color) {
  for (int i = 0; i < 8; i++) {
    unsigned char l = letter[i];
    if (l & 0b10000000) drawPixel(img, x + i, y + 0, color);
    if (l & 0b01000000) drawPixel(img, x + i, y + 1, color);
    if (l & 0b00100000) drawPixel(img, x + i, y + 2, color);
    if (l & 0b00010000) drawPixel(img, x + i, y + 3, color);
    if (l & 0b00001000) drawPixel(img, x + i, y + 4, color);
    if (l & 0b00000100) drawPixel(img, x + i, y + 5, color);
    if (l & 0b00000010) drawPixel(img, x + i, y + 6, color);
    if (l & 0b00000001) drawPixel(img, x + i, y + 7, color);
  }
}
