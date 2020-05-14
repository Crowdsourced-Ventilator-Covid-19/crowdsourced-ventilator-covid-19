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
        ModScreen(Adafruit_HX8357 &tft, QueueHandle_t screenQ, QueueHandle_t settingQ);
        void draw(String label, int minv, int maxv, int val, int scale);
        void handleTouch(TSPoint p);
    private:
        void drawBackButton();
        void drawSlider(int minv, int maxv, int val);
        Adafruit_HX8357 *tft;
        QueueHandle_t settingQ;
        QueueHandle_t screenQ;
        Settings_t settings;
        Screen screen;
        int val;
        int maxv;
        int minv;
        int scale;
        int oval;

};


#endif