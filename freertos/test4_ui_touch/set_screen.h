#ifndef SET_SCREEN_H
#define SET_SCREEN_H

#include <Adafruit_HX8357.h>
#include <Arduino.h>
#ifndef FONTS_h
#define FONTS_h
#include <Fonts/FreeSansBold24pt7b.h>
#include <Fonts/FreeSansBold12pt7b.h>
#include <Fonts/FreeSans12pt7b.h>
#endif
#include "config.h"
#include "types.h"
#include <TouchScreen.h>


class SetScreen
{
    public:
        SetScreen(Adafruit_HX8357 &tft, QueueHandle_t screenQ, QueueHandle_t settingQ);
        void draw();
        void handleTouch(TSPoint p);
    protected:
    private:
        void drawSetButton(String label, String label2, String value, unsigned int fcolor, unsigned int bcolor, int x, int y);
        void drawBackButton();
        Adafruit_HX8357 *tft;
        QueueHandle_t settingQ;
        QueueHandle_t screenQ;
        Screen screen;
        Settings_t settings;
};

#endif