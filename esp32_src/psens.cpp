#include "psens.h"

#define Addr 0x28

/* Pressure sensor class
 * Tracks pressure, average pressure over last 10 samples, and peak pressure
 */

Psens::Psens(int tca) {
    this->tca = tca;
    mux = I2cMux();
    for (int i=0; i<10; i++) {
        entries[i] = 0;
    }
    total = 0.0;
    idx = 0;
    peak = 0;
    p = 0;
    t = millis();
    offset = 0;
}

// read pressure with calibrated offset
void Psens::read() {
    p = (_read() - offset)  * 1.01972; // mbar to cmh2o
    t = millis();

    total = total - entries[idx];
    entries[idx] = p;
    total = total + p;
    idx = idx + 1;
    if (idx >= 10) { idx = 0; }
    avg = total / 10.0;
    if (p > peak) { peak = p; }
}

// read raw pressure with no offset
float Psens::_read() {
    mux.select(tca);
    unsigned int data[4];
    Wire.requestFrom(Addr, 4);
    if (Wire.available() == 4)
    {
        data[0] = Wire.read();
        data[1] = Wire.read();
        data[2] = Wire.read();
        data[3] = Wire.read();
    } else {
        return NAN;
    }
    float p_tmp = ((data[0] & 0x3F) * 256.0 + data[1]);
    p_tmp = ((p_tmp - 1638.0) / ((13107.0) / 100));
    return p_tmp; //mbar
}

// determine calibrated offset
void Psens::calibrate(Adafruit_HX8357 &tft) {
    tft.println("Calibrating AMS pressure on tca " + String(tca) + "...");
    offset = _read();
    while(isnan(offset)) {
        tft.println("Failed to read AMS pressure on tca " + String(tca) + ", retrying...");
        delay(500);
        offset = _read();
    }
    tft.println("AMS on tca " + String(tca) + " calibration offset = " + String(offset));
}