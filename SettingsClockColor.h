/*
 * Aurora: https://github.com/pixelmatix/aurora
 * Copyright (c) 2014 Jason Coon
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#ifndef SettingsClockColor_H
#define SettingsClockColor_H

#include <FastLED.h>
#include "Effects.h"
#include "Drawable.h"
#include "Runnable.h"
#include "ClockDisplay.h"
#include "Settings.h"

// Forward declarations
extern ClockDisplay clockDisplay;
extern ClockDigitalShort clockDigitalShort;
extern SMLayerBackground<rgb24, 0> backgroundLayer;

class SettingsClockColor : public Runnable {
  private:
    int cursorX = 0;
    int cursorY = 0;
    bool hasChanges = false;
    unsigned long lastUpdate = 0;
    const unsigned int updateInterval = 100; // ms

  public:

    void start() {
      // Save green color at startup
      CRGB green = CRGB(0, 255, 0); // Pure green
      save(green);
      
      // Find cursor position for the saved color
      CHSV chsv;
      CRGB crgb;
      for (int x = 0; x < MATRIX_WIDTH - 1; x += 1) {
        uint8_t hue = x * 8;

        for (int y = 0; y < MATRIX_HEIGHT; y += 1) {
          if (y <= 16) {
            chsv = CHSV(hue, y * 15 + 15, 255);
          }
          else {
            chsv = CHSV(hue, 255, 255 - (y - 17) * 15);
          }

          hsv2rgb_rainbow(chsv, crgb);
          if (crgb.r == clockDisplay.color.red &&
              crgb.g == clockDisplay.color.green &&
              crgb.b == clockDisplay.color.blue) {
            cursorX = x;
            cursorY = y;
            return;
          }
        }
      }
    }

    void run() {
      while (true) {
        unsigned long now = millis();
        if (now - lastUpdate >= updateInterval) {
          drawFrame();
          lastUpdate = now;
        }

        // draw an RGB grid, Hue from 0 to 248 left to right (256 is the same as 0)
        // Saturation from 15 (mostly white) to 255 (fully-saturated color) top to middle (0 to 16), skipping 0 which is white and will be handled in the right-most column)
        // then Value from 255 (fully-saturated color) to 45 (mostly black) middle to bottom, skipping anything lower than 45 which is indistinguishable from black.
        CHSV chsv;
        CRGB crgb;
        CRGB selectedColor = CRGB::White;
        for (int x = 0; x < MATRIX_WIDTH - 1; x += 1) {
          uint8_t hue = x * 8;

          for (int y = 0; y < MATRIX_HEIGHT; y += 1) {
            if (y <= 16) {
              chsv = CHSV(hue, y * 15 + 15, 255);
            }
            else {
              chsv = CHSV(hue, 255, 255 - (y - 17) * 15);
            }

            hsv2rgb_rainbow(chsv, crgb);
            backgroundLayer.drawPixel(x, y, crgb);

            if (x == cursorX && y == cursorY)
              selectedColor = crgb;
          }
        }

        // draw a grayscale strip down the right edge
        // from 0 (white) to 69 (mostly black), skipping anything lower than 69 which is almost indistinguishable from black.
        int x = MATRIX_WIDTH - 1;
        for (int y = 0; y < MATRIX_HEIGHT; y += 1) {
          chsv = CHSV(0, 0, 255 - y * 6);
          hsv2rgb_rainbow(chsv, crgb);
          backgroundLayer.drawPixel(x, y, crgb);

          if (x == cursorX && y == cursorY)
            selectedColor = crgb;
        }

        // draw the cursor crosshairs
        uint8_t v = cursorY * 8;
        crgb = CRGB(v, v, v);
        // horizontal cursor lines
        backgroundLayer.drawLine(cursorX - 4, cursorY, cursorX - 2, cursorY, crgb);
        backgroundLayer.drawLine(cursorX + 4, cursorY, cursorX + 2, cursorY, crgb);
        // vertical cursor lines
        backgroundLayer.drawLine(cursorX, cursorY - 4, cursorX, cursorY - 2, crgb);
        backgroundLayer.drawLine(cursorX, cursorY + 4, cursorX, cursorY + 2, crgb);

        // draw the clock numbers at the bottom, so we can see the selected color better, without leaving the settings item
        clockDisplay.setColor(selectedColor);
        clockDisplay.readTime();

        if (cursorY >= 18) {
          clockDigitalShort.drawFrame(0);
        }
        else {
          clockDigitalShort.drawFrame(23);
        }

        indexedLayer.swapBuffers();
        backgroundLayer.swapBuffers();

        InputCommand command = readCommand(defaultHoldDelay);

        switch (command) {
          case InputCommand::Left:
            cursorX -= 1;
            hasChanges = true;
            break;

          case InputCommand::Right:
            cursorX += 1;
            hasChanges = true;
            break;

          case InputCommand::Up:
            cursorY -= 1;
            hasChanges = true;
            break;

          case InputCommand::Down:
            cursorY += 1;
            hasChanges = true;
            break;

          case InputCommand::Select:
          case InputCommand::Back:
            if (hasChanges) {
              // Get current color at cursor position
              CHSV chsv;
              CRGB crgb;
              
              if (cursorX == MATRIX_WIDTH - 1) {
                chsv = CHSV(0, 0, 255 - cursorY * 6);
              } else {
                if (cursorY <= 16) {
                  chsv = CHSV(cursorX * 8, cursorY * 15 + 15, 255);
                } else {
                  chsv = CHSV(cursorX * 8, 255, 255 - (cursorY - 17) * 15);
                }
              }
              
              // Convert to RGB and save
              hsv2rgb_rainbow(chsv, crgb);
              
              // Clear buffers before saving
              backgroundLayer.fillScreen(CRGB(CRGB::Black));
              backgroundLayer.swapBuffers();
              
              // Save with clean state
              save(crgb);
              hasChanges = false;
            }
            return;

          default:
            break;
        }

        if (cursorX < 0)
          cursorX = MATRIX_WIDTH - 1;
        else if (cursorX >= MATRIX_WIDTH)
          cursorX = 0;

        if (cursorY < 0)
          cursorY = MATRIX_HEIGHT - 1;
        else if (cursorY >= MATRIX_HEIGHT)
          cursorY = 0;
      }
    }

    void save(CRGB crgb) {
      // Store color components separately to avoid potential memory issues
      uint8_t r = crgb.r;
      uint8_t g = crgb.g;
      uint8_t b = crgb.b;
      
      // Create fresh rgb24 object for the color
      rgb24 color;
      color.red = r;
      color.green = g;
      color.blue = b;
      
      // Set the color using the fresh rgb24 object
      clockDisplay.setColor(color);
      
      // Save to SD card
      clockDisplay.saveColor();
      
      // Force a refresh of the clock time
      clockDisplay.readTime();
    }
};

#endif
