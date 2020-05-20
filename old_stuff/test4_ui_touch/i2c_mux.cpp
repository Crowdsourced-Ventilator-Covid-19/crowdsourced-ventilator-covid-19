#include "i2c_mux.h"

I2cMux::I2cMux() {
}

void I2cMux::select(uint8_t i) {
    if (i > 7) return;
    Wire.beginTransmission(TCAADDR);
    Wire.write(1 << i);
    Wire.endTransmission();
}