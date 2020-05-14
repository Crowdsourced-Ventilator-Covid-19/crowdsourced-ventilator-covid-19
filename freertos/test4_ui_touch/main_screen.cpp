#include "main_screen.h"
#include <Adafruit_HX8357.h>
#include "graph.h"

#define BLACK     0x0000
#define GREY      0xC618
#define WHITE     0xFFFF
#define RED       0xF800
#define YELLOW    0xFFE0
#define GREEN     0x07E0
#define DKBLUE    0x000D

MainScreen::MainScreen(Adafruit_HX8357 &tft, QueueHandle_t screenQ, QueueHandle_t stateQ) {
    this->tft = &tft;
    this->stateQ = stateQ;
    this->screenQ = screenQ;
    pGraph = Graph(tft, 40, 90, 320, 80, 0, 15, 1, -10, 50, 10, "Pressure", "", "cmH2o", DKBLUE, RED, YELLOW, WHITE, BLACK, false);
    fGraph = Graph(tft, 40, 190, 320, 80, 0, 15, 1, 0, 800, 200, "Volume", "", "ml", DKBLUE, RED, GREEN, WHITE, BLACK, false);
    vGraph = Graph(tft, 40, 290, 320, 85, 0, 15, 1, -60, 60, 20, "Flow", "", "lpm", DKBLUE, RED, WHITE, WHITE, BLACK, true);
};

void MainScreen::draw() {
    tft->fillScreen(BLACK);
    pGraph.draw();
    fGraph.draw();
    vGraph.draw();
    drawMeas("Ppeak cmh2o", 380, 380, 10, "", YELLOW);
    drawMeas("Pplat cmh2o", 380, 380, 60, "", YELLOW);
    drawMeas("PEEP cmh2o", 380, 380, 110, "", YELLOW);
    drawMeas("RR bpm", 380, 380, 160, "", WHITE);
    drawMeas("TV ml", 380, 380, 210, "", GREEN);
    drawMeas("MV lpm", 380, 380, 260, "", GREEN);
}

void MainScreen::update(Sample_t sample) {
    double t = float(sample.t % 15000) / 1000.0;
    pGraph.plot(sample.t, cos(t) * 10 + 20);
    fGraph.plot(sample.t, sin(t) * 200 + 300);
    vGraph.plot(sample.t, cos(t) * 40);
}

void MainScreen::updateMeas(State_t state) {
    updateVal(380, 10, String(round(state.peak)), YELLOW);
    updateVal(380, 60, String(round(state.plat)), YELLOW);
    updateVal(380, 110, String(round(state.peep)), YELLOW);
    updateVal(380, 160, String(state.rr), WHITE);
    updateVal(380, 210, String(round(state.tv)), GREEN);
    updateVal(380, 260, String(state.minvol / 1000.0), GREEN);
}

// draw measurement info
void MainScreen::drawMeas(String label, int labelx, int valuex, int y, String value, unsigned int vcolor) {
    tft->setFont();
    tft->setTextSize(1);
    tft->setTextColor(WHITE, BLACK);
    tft->setCursor(labelx, y);
    tft->println(label);
    updateVal(valuex, y, value, vcolor);
}
// update the measurement number
void MainScreen::updateVal(int x, int y, String value, unsigned int vcolor) {
    tft->setTextSize(1);
    tft->setFont(&FreeSans18pt7b);
    tft->setTextColor(vcolor, BLACK);
    tft->fillRect(x, y + 15, 100, 30, BLACK);
    tft->setCursor(x, y + 40);
    tft->println(value);
}

void MainScreen::handleTouch(TSPoint p) {
    screen = SETSCREEN;
    xQueueOverwrite(screenQ, &screen);
}
