#include "sim_fsens.h"

SimFsens::SimFsens(QueueHandle_t lungQ, int tca) {
    this->lungQ = lungQ;
    this->tca = tca;
    v = 0;
    of = 0;
    ot = millis();
    t = millis();

    for(int i=0; i<60; i++) {
        vals[i] = 0;
        ts[i] = millis();
    }
    idx = 0;
}

void SimFsens::read() {
    if(xQueuePeek(lungQ, &lung,10) == pdTRUE) {
        if (tca == 0) {
            f = lung.fin;
        } else if (tca == 2) {
            f = lung.fout;
        }
        t = millis();

        // handle time wraparound case
        uint32_t dt;
        if (t < ot) {
            dt = UINT32_MAX - ot + t;
        } else {
            dt = t - ot;
        }
        // trapezoidal intgegration to track volume (lpm*s -> cc)
        // Serial.println("dt: " + String(dt) + " of: " + String(of) + " tmpF: " + String(tmpF) + " tmpTv: " + String(tmpTv));
        v += float(dt) * (of + f) / 2.0 / 60.0;
        of = f;
        ot = t;
    }
}

void SimFsens::updateMv() {
    vals[idx] = v;
    ts[idx] = t;
    idx = idx + 1;
    if (idx >= 60) { idx = 0; }
    float mv = 0;
    uint32_t dt;
    for(int i=0; i<60; i++) {
        if (t < ts[i]) {
            dt = UINT32_MAX - ts[i] + t;
        } else {
            dt = t - ts[i];
        }
        if(dt <= 60 * 1000) {
            mv += vals[i];
        }
    }
}

