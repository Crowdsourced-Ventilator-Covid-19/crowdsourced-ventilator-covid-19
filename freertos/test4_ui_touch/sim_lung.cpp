#include "sim_lung.h";

Lung::Lung(QueueHandle_t stateQ, QueueHandle_t lungQ) {
    this->stateQ = stateQ;
    this->lungQ = lungQ;
    this->oldPhase = NOPHASE;
    t0 = 0;
    ti = 0;
    tpi = 0;
    te = 0;

}

void Lung::update() {
    if (xQueuePeek(stateQ, &state, 10) == pdTRUE) {
        uint32_t t = millis();
        if (oldPhase !=INSPIRATORY && state.phase == INSPIRATORY ) {
            t0 = t;
            tpi = 0;
            te = 0;
        }
        switch(state.phase) {
            case INSPIRATORY:
                ti = t - t0;
                break;
            case POSTINSPIRATORY:
                tpi = std::max(t - t0 - ti, (uint32_t) 0);
                break;
            case EXPIRATORY:
                te = std::max(t - t0 - ti - tpi, (uint32_t) 0);
                break;
        }
        oldPhase = state.phase;
        float p_i = -40 * (exp(-0.002*float(ti)) -1);
        float p_pi = -2 + 2*exp(-0.01*tpi);
        float p_pe = (p_i + p_pi) * (exp(-0.005*te) - 1);
        lung.p = p_i + p_pi + p_pe;
        if (state.phase == INSPIRATORY) {
            if (t - t0 < 333) {
                lung.fin = 0.8 * 3.0 * float(t-t0)/1000 * 60;
            } else {
                lung.fin = 0.8 * exp(-0.005*float(t-t0-333)) * 60;
            }
            lung.fout = 0;
        } else if (state.phase == EXPIRATORY) {
            if (te < 333) {
                lung.fout = 0.8 * 3.0 *  float(te)/1000 * 60;
            } else {
                lung.fout = 0.8 * exp(-0.005*float(te)) * 60;
            }
            lung.fin = 0;
        }

        //char buffer [100];
        //sprintf(buffer, "%d %d %d %d %d %d %0.2f %0.2f %0.2f %0.2f", state.phase, t, t0, ti, tpi, te, p_i, p_pi, p_pe, lung.p);
        //sprintf(buffer, "%d %d %d %0.2f %0.2f", state.phase, t-t0, te, lung.fin, lung.fout);
        //Serial.println(buffer);
        
        xQueueOverwrite(lungQ, &lung);
    }

}