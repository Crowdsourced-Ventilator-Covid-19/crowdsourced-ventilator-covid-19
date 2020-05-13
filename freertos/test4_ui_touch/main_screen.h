#ifndef MAIN_SCREEN_H
#define MAIN_SCREEN_H

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
#include "graph.h"

class MainScreen
{
    public:
        MainScreen(Adafruit_HX8357 &tft);
        void draw();
        void update(Sample_t sample);
    private:
        Adafruit_HX8357 *tft;
        State_t state;
        Settings_t settings;
        Sample_t sample;
        Graph pGraph;
        Graph fGraph;
        Graph vGraph;

};

#endif