#ifndef FSENS_H
#define FSENS_H

#include <Arduino.h>
#include <Adafruit_HX8357.h>
#include "i2c_mux.h"
#include "config.h"

class Fsens
{
    public:
        Fsens(int tca);
        void read();
        void updateMv();
        void calibrate(Adafruit_HX8357 &tft);
        float f;
        uint32_t t;
        float v;
        float mv;
        int rr;
    private:
        float _read();
        int tca;
        uint32_t ot;
        float of;
        float vals[60];
        uint32_t ts[60];
        int idx;
        I2cMux mux;
        float offset;
};

#endif