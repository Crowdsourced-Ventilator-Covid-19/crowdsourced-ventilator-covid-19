#include "set_screen.h"
#include <Adafruit_HX8357.h>
#include "config.h"

#define BLACK     0x0000
#define GREY      0xC618
#define WHITE     0xFFFF
#define RED       0xF800
#define YELLOW    0xFFE0
#define GREEN     0x07E0

#define TV_MIN     100
#define TV_MAX     800
#define RR_MIN     10
#define RR_MAX     40
#define IE_MAX     30
#define IE_MIN     10
#define PIP_MAX    60
#define PIP_MIN    10
#define TRIG_MAX   100
#define TRIG_MIN   0
#define PIP_ALRM_MAX 700
#define PIP_ALRM_MIN 100
#define MVHI_ALRM_MAX 30
#define MVHI_ALRM_MIN 0
#define MVLO_ALRM_MAX 30
#define MVLO_ALRM_MIN 0
#define DC_ALRM_MAX 20
#define DC_ALRM_MIN 0

SetScreen::SetScreen(Adafruit_HX8357 &tft, Screen &screen, QueueHandle_t settingQ, ModVal_t &modvals) {
    this->tft = &tft;
    this->settingQ = settingQ;
    this->screen = &screen;
    this->modvals = &modvals;
    xQueuePeek(settingQ, &settings, 0);
    this->lastDebounceTime = 0;
    this->debounceDelay = 500;
};

void SetScreen::draw() {
    updateSettings();
    tft->fillScreen(BLACK);
    drawBackButton();
    tft->setFont(&FreeSansBold24pt7b);
    tft->setTextColor(WHITE, BLACK);
    tft->setCursor(100, 50);
    tft->println("Settings");
    String powertxt = (settings.power)? " STP" : " RUN";  // I have no idea why there are spaces here
    char buffer[5];
    sprintf(buffer, "1:%2.1f", float(settings.ier) / 10.0);
    drawSetButton("RR", "", String(settings.rr),  BLACK, WHITE, 4, 70);
    drawSetButton("TV", "", String(settings.tv), BLACK, GREEN, 123, 70);
    drawSetButton("I/E", "", buffer, BLACK, WHITE, 242, 70);
    drawSetButton("Pmax", "", String(settings.pmax), BLACK, YELLOW, 361, 70);
    drawSetButton("AC", "Trig", String(settings.trig), BLACK, YELLOW, 4, 194);
    drawSetButton("Power", "", powertxt, BLACK, WHITE, 123, 194);
    drawSetButton("Alarms", "", String(""), BLACK, RED, 361, 194);
};

void SetScreen::drawSetButton(String label, String label2, String value, unsigned int fcolor, unsigned int bcolor, int x, int y) {
    tft->fillRoundRect(x, y, 115, 120, 7, bcolor);
    tft->setTextSize(1);
    tft->setFont(&FreeSans12pt7b);
    tft->setTextColor(fcolor, bcolor);
    tft->setCursor(x + 20, y + 30);
    tft->println(label);
    tft->setCursor(x + 20, y + 60);
    tft->println(label2);
    tft->setFont(&FreeSansBold24pt7b);
    tft->setCursor(x+10, y+110);
    tft->println(value);
};

void SetScreen::drawBackButton() {
    tft->fillRect(361, 4, 115, 62, GREY);
    tft->fillTriangle(361, 66, 335, 35, 361, 4, GREY);
    tft->setTextSize(1);
    tft->setFont(&FreeSansBold12pt7b);
    tft->setTextColor(BLACK, GREY);
    tft->setCursor(380, 45);
    tft->println("BACK");
};

void SetScreen::handleTouch(TSPoint p) {
    // xQueuePeek(settingQ, &settings, 10);
    if (p.x > 330 && p.y < 66) { // back button
        xQueueOverwrite(settingQ, &settings);
        *screen = MAINSCREEN;
    } else if (p.x < 120 && p.y > 194) { // trig
        *modvals = {"AC Trig", TRIG_MIN, TRIG_MAX, settings.trig, settings.trig};
        *screen = MODSCREEN;
    } else if (p.x < 240 && p.y > 194) { // power
        if ((millis() - lastDebounceTime) > debounceDelay) {
            settings.power = !settings.power;
            String powertxt = (settings.power)? "STP" : "RUN ";
            //tft->fillRect(123,220,120,100,BLACK);
            drawSetButton("Power", "", powertxt, BLACK, WHITE, 123, 194);
            xQueueOverwrite(settingQ, &settings);
            lastDebounceTime = millis();
        }
    } else if (p.x < 360 && p.y > 194) {

    } else if (p.y > 194) {

       // drawAlarmScreen();
    } else if (p.x < 120 && p.y > 70) {
        *modvals = {"RR", RR_MIN, RR_MAX, settings.rr, settings.rr};
        *screen = MODSCREEN;
    } else if (p.x < 240 && p.y > 70) {
        *modvals = {"TV", TV_MIN, TV_MAX, settings.tv, settings.tv};
        *screen = MODSCREEN;
    } else if (p.x < 360 && p.y > 70) {
        *modvals = {"I/E Ratio", IE_MIN, IE_MAX, settings.ier, settings.ier};
        *screen = MODSCREEN;
    } else if (p.y > 70) {
        *modvals = {"Pmax", PIP_MIN, PIP_MAX, settings.pmax, settings.pmax};
        *screen = MODSCREEN;
    } 
}

void SetScreen::updateSettings() {
    if (modvals->newval != modvals->val) {
        if (modvals->label == "AC Trig") {
            settings.trig = modvals->newval;
        } else if (modvals->label == "RR") {
            settings.rr = modvals->newval;
        } else if (modvals->label == "TV") {
            settings.tv = modvals->newval;
        } else if (modvals->label == "I/E Ratio") {
            settings.ier = modvals->newval;
        } else if (modvals->label == "Pmax") {
            settings.pmax = modvals->newval;
        }
    }
}