#ifndef SET_SCREEN_H
#define SET_SCREEN_H

#include <Adafruit_GFX.h>    // Core graphics library
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


class SetScreen
{
    public:
        SetScreen(Adafruit_HX8357 &tft, QueueHandle_t stateQ, QueueHandle_t settingQ);
        void clear();
        void drawSetScreen();
    protected:
    private:
        void drawSetButton(String label, String label2, String value, unsigned int fcolor, unsigned int bcolor, int x, int y);
        void drawBackButton();
        Adafruit_HX8357 *tft;
        QueueHandle_t stateQ;
        QueueHandle_t settingQ;
        State_t state;
        Settings_t settings;
};

#endif