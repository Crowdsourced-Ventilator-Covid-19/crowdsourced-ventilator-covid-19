#ifndef SIM_FSENS_H
#define SIM_FSENS_H
#include <Arduino.h>
#include "types.h"
#include "i2c_mux.h"

class SimFsens
{
    public:
        SimFsens(QueueHandle_t lungQ, int tca);
        void read();
        void updateMv();
        float f;
        uint32_t t;
        float v;
        float mv;
        int rr;
    private:
        QueueHandle_t lungQ;
        Lung_t lung;
        int tca;
        uint32_t ot;
        float of;
        float vals[60];
        uint32_t ts[60];
        int idx;
        I2cMux mux;
};

#endif