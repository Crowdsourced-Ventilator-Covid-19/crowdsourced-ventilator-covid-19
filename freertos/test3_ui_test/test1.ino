#define ARDUINO 10812
#include <Arduino.h>
#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_HX8357.h>
#include <Adafruit_MPRLS.h>
#include <TouchScreen.h>
#include <SPI.h>
#include "config.h"
#include "types.h"
#ifndef FONTS_h
#define FONTS_h
#include <Fonts/FreeSansBold24pt7b.h>
#include <Fonts/FreeSansBold12pt7b.h>
#include <Fonts/FreeSans12pt7b.h>
#endif
#include "graph.h"

#define RESET_PIN -1
#define EOC_PIN -1
Adafruit_MPRLS mpr = Adafruit_MPRLS(RESET_PIN, EOC_PIN);

Adafruit_HX8357 tft = Adafruit_HX8357(TFT_CS, TFT_DC, TFT_RST);
// For better pressure precision, we need to know the resistance
// between X+ and X- Use any multimeter to read it
// For the one we're using, its 300 ohms across the X plate
TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);




QueueHandle_t sampleQ = NULL;
QueueHandle_t settingQ = NULL;
QueueHandle_t stateQ = NULL;

void setup(void) {
    Serial.begin(115200);
    delay(1000);
    tft.begin();
    tft.setRotation(3);
    tft.fillScreen(BLACK);

    // touchscreen library expects 10-bit voltage measurements
    analogReadResolution(10);

    sampleQ = xQueueCreate(1000, sizeof(Sample_t));
    settingQ = xQueueCreate(1, sizeof(Settings_t));
    stateQ = xQueueCreate(1, sizeof(State_t));

    initSettings();

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
        sample = {millis(), mpr.readPressure() * 0.0101972};
        if(xQueueSend(sampleQ, &sample, 0) != pdTRUE) {
            Serial.println("queue error\n\r");
        }
    }
}

// print sensor results
void displayResults( void * parameter)
{
    int display = 2;               // redraw level 2 means draw axes and chart
    double ox1 = -999, oy1 = -999;  // Force them to be off screen
    double ox2 = -999, oy2 = -999; 
    double ox3 = -999, oy3 = -999; 
    double last_t = 999;           // track wraparound
    double t;

    Sample_t sample;
    for(;;) {
        if(xQueueReceive(sampleQ, &sample,100) == pdTRUE) {
            Serial.println(String(sample.t) + " " + String(sample.v));
            t = float(sample.t % 15000) / 1000.0;  // convert millis time to seconds mod 15
            if(t < last_t) { display = 2; } else { display = 0; }
            Graph(tft, t, cos(t) * 10 + 20, 40, 90, 320, 80, 0, 15, 1, -10, 50, 10, "Pressure", "", "cmH2o", DKBLUE, RED, YELLOW, WHITE, BLACK, display, &ox1, &oy1);
            Graph(tft, t, sin(t) * 200 + 300, 40, 190, 320, 80, 0, 15, 1, 0, 800, 200, "Volume", "", "cc", DKBLUE, RED, GREEN, WHITE, BLACK, display, &ox2, &oy2);
            Graph(tft, t, cos(t) * 40, 40, 290, 320, 85, 0, 15, 1, -60, 60, 20, "Flow", "", "lpm", DKBLUE, RED, WHITE, WHITE, BLACK, display, &ox3, &oy3);
            last_t = t;
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

void initSettings() {
    Settings_t init = {
        15,
        1.0,
        40,
        30,
        20,
        1,
        3,
        300,
        50
    };
    xQueueOverwrite(settingQ, &init);
}


