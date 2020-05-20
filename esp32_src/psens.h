#ifndef PSENS_H
#define PSENS_H
#include <Arduino.h>
#include "i2c_mux.h"
#include "Wire.h"
#include <Adafruit_HX8357.h>


class Psens
{
    public:
        Psens(int tca);
        void read();
        void calibrate(Adafruit_HX8357 &tft);
        float avg;
        float peak;
        float p;
        uint32_t t;
    private:
        float _read();
        float entries[10];
        float total;
        int idx;
        int tca;
        I2cMux mux;
        float offset;
};

#endif
