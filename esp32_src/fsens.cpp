#include "fsens.h"

#define Addr 0x28

/* flow sensor class
 * tracks flow, tidal volume, repiratory rate and minute volume
 */


Fsens::Fsens(int tca) {
    this->tca = tca;
    mux = I2cMux();
    v = 0;
    of = 0;
    ot = millis();
    t = millis();

    for(int i=0; i<60; i++) {
        vals[i] = 0;
        ts[i] = 1000*60*60;  // make sure these timestamps won't count towards RR
    }
    idx = 0;
    offset = 0;
}

// read flow
void Fsens::read() {
    mux.select(tca);
    float r1, r2, A1, A2, rho, deltaP, flow;
    // venturi flow meter constants
    r1 = 0.0095;        // outer radius (m)
    r2 = 0.00189;       // inner radius (m)
    A1 = PI * sq(r1);   // outer area (m^2)
    A2 = PI * sq(r2);   // inner area (m^2)
    rho = 1.225;        // density of air @ sea level (kg/m^3)

    // Flow calculation from https://www.engineeringtoolbox.com/orifice-nozzle-venturi-d_590.html
    deltaP = (_read() - offset) * 100.0; // mbar to N/m2
    t = millis();
    if (deltaP < 0) { deltaP = 0; } // dont measure negative flow

    flow = A2 * sqrt(2 * deltaP / rho / (1 - sq(A2/A1))) * 1000 * 60;

    // If flow is below flowDeadZone in magnitude, ignore it to avoid integration error
    if (abs(flow) < flowDeadZone) { flow = 0; }
    f = flow;

    // trapezoidal intgegration to track volume (lpm*s -> cc)
    v += float(t - ot) * (of + f) / 2.0 / 60.0;
    of = f;
    ot = t;
}

void Fsens::updateMv() {
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

// read pressure without calibrated offset
float Fsens::_read() {
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
void Fsens::calibrate(Adafruit_HX8357 &tft) {
    tft.println("Calibrating AMS pressure on tca " + String(tca) + "...");
    offset = _read();
    while(isnan(offset)) {
        tft.println("Failed to read AMS pressure on tca " + String(tca) + ", retrying...");
        delay(500);
        offset = _read();
    }
    tft.println("AMS on tca " + String(tca) + " calibration offset = " + String(offset));
}