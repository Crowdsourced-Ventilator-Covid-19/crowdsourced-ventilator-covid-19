#ifndef LUNG_H
#define LUNG_H
#include <Arduino.h>
#include "types.h";

class Lung
{
    public:
        Lung(QueueHandle_t stateQ, QueueHandle_t lungQ);
        void update();
    private:
        QueueHandle_t stateQ;
        QueueHandle_t lungQ;
        State_t state;
        Lung_t lung;
        uint32_t t0;  // start of inspiratory breath
        uint32_t ti;  // inspiration timer
        uint32_t tpi; // post-inspiration timer
        uint32_t te;  // expiration timer
        Phase oldPhase;
};


#endif