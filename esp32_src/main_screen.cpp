#include "main_screen.h"
#include <Adafruit_HX8357.h>
#include "graph.h"
#include <Fonts/FreeSans12pt7b.h>

#define BLACK     0x0000
#define GREY      0xC618
#define WHITE     0xFFFF
#define RED       0xF800
#define YELLOW    0xFFE0
#define GREEN     0x07E0
#define DKBLUE    0x000D

MainScreen::MainScreen(Adafruit_HX8357 &tft, Screen &screen) {
    this->tft = &tft;
    this->screen = &screen;
    pGraph = Graph(tft, 40, 90, 320, 80, 0, 15, 1, -10, 50, 10, "Pressure", "", "cmH2o", DKBLUE, RED, YELLOW, WHITE, BLACK, false);
    vGraph = Graph(tft, 40, 190, 320, 80, 0, 15, 1, 0, 800, 200, "Volume", "", "ml", DKBLUE, RED, GREEN, WHITE, BLACK, false);
    fGraph = Graph(tft, 40, 290, 320, 85, 0, 15, 1, -100, 100, 25, "Flow", "", "lpm", DKBLUE, RED, WHITE, WHITE, BLACK, true);
    alarmState = 0;
};

void MainScreen::draw() {
    tft->fillScreen(BLACK);
    pGraph.draw();
    fGraph.draw();
    vGraph.draw();
    drawMeas("Ppeak cmh2o", 380, 10);
    drawMeas("Pplat cmh2o", 380, 60);
    drawMeas("PEEP cmh2o", 380, 110);
    drawMeas("RR bpm", 380, 160);
    drawMeas("TV ml", 380, 210);
    drawMeas("MV lpm", 380, 260);
}

void MainScreen::updateAlarms(Alarm_t alarms) {
    if ((alarms.pmax || alarms.mvhi || alarms.mvlo || alarms.dc) && (millis() > pulseTimer)) {
        alarmState = !alarmState;
        if (alarmState) {
            fcolor = BLACK;
        } else {
            fcolor = RED;
        }
        tft->setTextSize(1);
        tft->setFont(&FreeSans12pt7b);
        tft->setTextColor(fcolor, BLACK);
        if (alarms.pmax) {
            tft->setCursor(220 , 40);
            tft->println("PMax ALRM");
        } else if (alarms.pwarn) {
            tft->setCursor(220 , 40);
            tft->println("PMax WARN");
        } else if (alarms.mvhi) {
            tft->setCursor(220, 140);
            tft->println("MvHi ALRM");
        } else if (alarms.mvlo) {
            tft->setCursor(220, 140);
            tft->println("MvLo ALRM");
        } else if (alarms.dc) {
            tft->setCursor(220, 240);
            tft->println("DC ALRM");
        }
        pulseTimer = millis() + 1000;
    }
}

void MainScreen::update(Sample_t sample) {
    pGraph.plot(sample.p_ts, sample.p);
    fGraph.plot(sample.f_ts, sample.f);
    vGraph.plot(sample.v_ts, sample.v);
}

void MainScreen::updateMeas(State_t state) {
    updateVal(380, 10, String(int(round(state.peak))), YELLOW);
    updateVal(380, 60, String(int(round(state.plat))), YELLOW);
    updateVal(380, 110, String(int(round(state.peep))), YELLOW);
    updateVal(380, 160, String(int(round(state.rr))), WHITE);
    updateVal(380, 210, String(int(round(state.tv))), GREEN);
    updateVal(380, 260, String(state.minvol / 1000.0), GREEN);
}

// draw measurement info
void MainScreen::drawMeas(String label, int labelx, int y) {
    tft->setFont();
    tft->setTextSize(1);
    tft->setTextColor(WHITE, BLACK);
    tft->setCursor(labelx, y);
    tft->println(label);
}

// update the measurement number
void MainScreen::updateVal(int x, int y, String value, unsigned int vcolor) {
    // Serial.println(String(value));
    tft->setTextSize(1);
    tft->setFont(&FreeSans18pt7b);
    tft->setTextColor(vcolor, BLACK);
    tft->fillRect(x, y + 15, 100, 30, BLACK);
    tft->setCursor(x, y + 40);
    tft->println(value);
}

void MainScreen::handleTouch(TSPoint p) {
    *screen = SETSCREEN;
}
