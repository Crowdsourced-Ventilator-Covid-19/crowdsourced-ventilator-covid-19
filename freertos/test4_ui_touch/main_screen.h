#ifndef MAIN_SCREEN_H
#define MAIN_SCREEN_H

#include <Adafruit_HX8357.h>
#include <Arduino.h>
#ifndef FONTS_h
#define FONTS_h
#include <Fonts/FreeSansBold24pt7b.h>
#include <Fonts/FreeSansBold12pt7b.h>
#include <Fonts/FreeSans18pt7b.h>
#endif
#include "types.h"
#include "graph.h"
#include <TouchScreen.h>

class MainScreen
{
    public:
        MainScreen(Adafruit_HX8357 &tft, QueueHandle_t);
        void draw();
        void update(Sample_t sample);
        void updateMeas(State_t state);
        void handleTouch(TSPoint p);
    private:
        void drawMeas(String label, int labelx, int valuex, int y, String value, unsigned int vcolor);
        void updateVal(int x, int y, String value, unsigned int vcolor);
        QueueHandle_t stateQ;
        Adafruit_HX8357 *tft;
        State_t state;
        Settings_t settings;
        Sample_t sample;
        Graph pGraph;
        Graph fGraph;
        Graph vGraph;

};

#endif