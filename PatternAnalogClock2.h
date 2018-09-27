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

#ifndef PatternAnalogClock2_H

class PatternAnalogClock2 : public Drawable {
private:
#define HOUR_RADIUS 16
#define MIN_RADIUS  26
#define SEC_RADIUS   28

public:
    PatternAnalogClock2() {
        name = (char *)"AnalogClock2";
    }

    void start() {
    }

    unsigned int drawFrame() {
        effects.DimAll(250);

        clockDisplay.readTime();

        if (isTimeAvailable) {
            uint8_t hour = time.Hour;
            if (hour > 12)
                hour -= 12;

            double radians;
            int hourx;
            int houry;
            int minx;
            int miny;
            int secx;
            int secy;
            
            // hour
            radians = (90.0F - ((hour * 30.0F) + ((time.Minute * 29.0F) / 60.0F))) * M_PI / 180.0F;
            hourx = round(MATRIX_CENTRE_X + HOUR_RADIUS * cos(radians));
            houry = round(MATRIX_CENTRE_Y - HOUR_RADIUS * sin(radians));
            effects.leds[XY(hourx, houry)] += {255, 0, 0};

            // minute
            radians = (90.0F - (time.Minute * 6.0F)) * M_PI / 180.0;
            minx = round(MATRIX_CENTRE_X + MIN_RADIUS * cos(radians));
            miny = round(MATRIX_CENTRE_Y - MIN_RADIUS * sin(radians));
            effects.leds[XY(minx, miny)] += {0, 255, 0};

            // second
            radians = (90.0F - ((float)time.Second * 6.0F)) * M_PI / 180.0F;
            secx = round(MATRIX_CENTRE_X + SEC_RADIUS * cos(radians));
            secy = round(MATRIX_CENTRE_Y - SEC_RADIUS * sin(radians));
            effects.leds[XY(secx, secy)] += {0, 0, 255};

            // millisecond
            //uint8_t xm = beatcos8(60, 0, MATRIX_CENTER_X + SEC_RADIUS);
            //uint8_t ym = beatsin8(60, 0, MATRIX_CENTER_Y + SEC_RADIUS);
            //effects.leds[XY(xm, ym)] += effects.ColorFromCurrentPalette(beat8(1));

            backgroundLayer.drawLine(MATRIX_CENTER_X, MATRIX_CENTER_Y, hourx, houry, {255, 0, 0});
            backgroundLayer.drawLine(MATRIX_CENTER_X, MATRIX_CENTER_Y, minx, miny, {0, 255, 0});
            backgroundLayer.drawLine(MATRIX_CENTER_X, MATRIX_CENTER_Y, secx, secy, {0, 0, 255});
            backgroundLayer.drawCircle(MATRIX_CENTER_X, MATRIX_CENTER_Y, 31, effects.ColorFromCurrentPalette(beat8(1)));
            
            backgroundLayer.setFont(font3x5);
            backgroundLayer.drawString(29, 3, { 255, 255, 255 }, "12");
            backgroundLayer.drawString(45, 8, { 255, 255, 255 }, "1");
            backgroundLayer.drawString(54, 18, { 255, 255, 255 }, "2");
            backgroundLayer.drawString(59, 30, { 255, 255, 255 }, "3");
            backgroundLayer.drawString(54, 42, { 255, 255, 255 }, "4");
            backgroundLayer.drawString(45, 52, { 255, 255, 255 }, "5");
            backgroundLayer.drawString(31, 57, { 255, 255, 255 }, "6");
            backgroundLayer.drawString(18, 52, { 255, 255, 255 }, "7");
            backgroundLayer.drawString(9, 42, { 255, 255, 255 }, "8");
            backgroundLayer.drawString(3, 30, { 255, 255, 255 }, "9");
            backgroundLayer.drawString(7, 18, { 255, 255, 255 }, "10");
            backgroundLayer.drawString(16, 8, { 255, 255, 255 }, "11");
        }

            
        
        else {
            backgroundLayer.setFont(font3x5);
            backgroundLayer.drawString(0, 0, { 255, 255, 255 }, "No Clock");
        }

        return 0;
    }
};

#endif
