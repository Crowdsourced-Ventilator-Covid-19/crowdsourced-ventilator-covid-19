#ifndef I2C_MUX_H
#define I2C_MUX_H

#include <Arduino.h>
#include "Wire.h"

#define TCAADDR 0x70

class I2cMux
{
    public:
        I2cMux();
        void select(uint8_t i);
};


#endif
