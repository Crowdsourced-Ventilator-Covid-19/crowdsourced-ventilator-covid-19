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
        ts[i] = 1000*60*60;  // make sure these timestamps won't count towards RR
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
        // trapezoidal intgegration to track volume (lpm*s -> cc)
        v += float(t - ot) * (of + f) / 2.0 / 60.0;
        of = f;
        ot = t;
    }
}

void SimFsens::updateMv() {
    vals[idx] = v;
    ts[idx] = t;
    idx += 1;
    if (idx >= 60) { idx = 0; }
    mv = 0;
    rr = 0;
    for(int i=0; i<60; i++) {
        if(t-ts[i] <= 60 * 1000) {
            mv += vals[i];
            rr += 1;
        }
    }
}

