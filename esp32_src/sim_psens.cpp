#include "sim_psens.h"

/* Pressure sensor class
 * Tracks pressure, average pressure over last 10 samples, and peak pressure
 */

SimPsens::SimPsens(QueueHandle_t lungQ, int tca) {
    this->lungQ = lungQ;
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
}

void SimPsens::read() {
    mux.select(tca);
    if(xQueuePeek(lungQ, &lung,10) == pdTRUE) {
        total = total - entries[idx];
        entries[idx] = lung.p;
        total = total + lung.p;
        idx = idx + 1;
        if (idx >= 10) { idx = 0; }
        avg = total / 10.0;
        if (lung.p > peak) { peak = lung.p; }
        p = lung.p;
        t = millis();
    }
}

// determine calibrated offset
void SimPsens::calibrate(Adafruit_HX8357 &tft) {
    tft.println("Calibrating AMS pressure on tca " + String(tca) + "...");
    offset = 0;
    while(isnan(offset)) {
        tft.println("Failed to read AMS pressure on tca " + String(tca) + ", retrying...");
        delay(500);
        offset = 0;
    }
    tft.println("AMS on tca " + String(tca) + " calibration offset = " + String(offset));
}