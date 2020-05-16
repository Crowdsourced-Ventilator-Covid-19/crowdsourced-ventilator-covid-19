#ifndef ALARM_SCREEN_H
#define ALARM_SCREEN_H

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


class AlarmScreen
{
    public:
        AlarmScreen(Adafruit_HX8357 &tft, Screen &screen, QueueHandle_t settingQ, ModVal_t &modvals);
        void draw();
        void handleTouch(TSPoint p);
    protected:
    private:
        void drawSetButton(String label, String label2, String value, unsigned int fcolor, unsigned int bcolor, int x, int y);
        void drawBackButton();
        void updateSettings();
        ModVal_t *modvals;
        Adafruit_HX8357 *tft;
        QueueHandle_t settingQ;
        Screen *screen;
        Settings_t settings;
        unsigned long lastDebounceTime;
        unsigned long debounceDelay;
};

#endif