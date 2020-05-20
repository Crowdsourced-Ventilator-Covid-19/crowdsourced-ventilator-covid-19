#define ARDUINO 10812
#include <Arduino.h>
#include "Adafruit_MPRLS.h"

#define RESET_PIN -1
#define EOC_PIN -1
Adafruit_MPRLS mpr = Adafruit_MPRLS(RESET_PIN, EOC_PIN);

typedef struct {
    uint32_t t;
    float v;
} Sample_t;

QueueHandle_t p1Queue = NULL;

void setup(void) {
    Serial.begin(115200);
    delay(1000);

    p1Queue = xQueueCreate(1000, sizeof(Sample_t));

    xTaskCreatePinnedToCore(
                    readSensor,          /* Task function. */
                    "ReadSensor",        /* String with name of task. */
                    10000,            /* Stack size in bytes. */
                    NULL,             /* Parameter passed as input of the task */
                    1,                /* Priority of the task. */
                    NULL,             /* Task handle. */
                    0);               /* pin to core 0 */
 
    xTaskCreatePinnedToCore(
                    displayResults,          /* Task function. */
                    "DisplayResults",        /* String with name of task. */
                    10000,            /* Stack size in bytes. */
                    NULL,             /* Parameter passed as input of the task */
                    1,                /* Priority of the task. */
                    NULL,             /* Task handle. */
                    1);               /* pin to core 1 */
}

void loop() {
    delay(1000);
}

// read sensor
void readSensor( void * parameter )
{
    initSensor();
    Sample_t sample;
    for(;;) {
        sample = {millis(), mpr.readPressure()};
        if(xQueueSend(p1Queue, &sample, 0) != pdTRUE) {
            Serial.println("queue error\n\r");
        }
    }
}

// print sensor results
void displayResults( void * parameter)
{
    Sample_t sample;
    for(;;) {
        if(xQueueReceive(p1Queue, &sample,100) == pdTRUE) {
            Serial.println(String(sample.t) + " " + String(sample.v));
        }
    }
 
}

void initSensor() {
    Serial.println("MPRLS Simple Test");
    if (! mpr.begin()) {
        Serial.println("Failed to communicate with MPRLS sensor, check wiring?");
        while (1) {
            delay(10);
        }
    }
    Serial.println("Found MPRLS sensor");
}