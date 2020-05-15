#ifndef MOD_SCREEN_H
#define MOD_SCREEN_H

#include <Adafruit_HX8357.h>
#include <Arduino.h>
#ifndef FONTS_h
#define FONTS_h
#include <Fonts/FreeSansBold24pt7b.h>
#include <Fonts/FreeSansBold12pt7b.h>
#include <Fonts/FreeSans12pt7b.h>
#endif
#include "types.h"
#include <TouchScreen.h>

class ModScreen
{
    public:
        ModScreen(Adafruit_HX8357 &tft, Screen &screen, ModVal_t &modvals);
        void draw();
        void handleTouch(TSPoint p);
    private:
        void drawBackButton();
        void drawSlider();
        void drawValue();
        Adafruit_HX8357 *tft;
        QueueHandle_t settingQ;
        ModVal_t *modvals;
        Screen *screen;
        int val;
        int oval;

};


#endif