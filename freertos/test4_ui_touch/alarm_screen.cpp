#include "alarm_screen.h"
#include <Adafruit_HX8357.h>
#include "config.h"

#define BLACK     0x0000
#define GREY      0xC618
#define WHITE     0xFFFF
#define RED       0xF800
#define YELLOW    0xFFE0
#define GREEN     0x07E0

#define PIP_ALRM_MAX 60
#define PIP_ALRM_MIN 10
#define MVHI_ALRM_MAX 30
#define MVHI_ALRM_MIN 0
#define MVLO_ALRM_MAX 30
#define MVLO_ALRM_MIN 0
#define DC_ALRM_MAX 20
#define DC_ALRM_MIN 0

AlarmScreen::AlarmScreen(Adafruit_HX8357 &tft, Screen &screen, QueueHandle_t settingQ, ModVal_t &modvals) {
    this->tft = &tft;
    this->settingQ = settingQ;
    this->screen = &screen;
    this->modvals = &modvals;
    xQueuePeek(settingQ, &settings, 0);
    this->lastDebounceTime = 0;
    this->debounceDelay = 500;
};

void AlarmScreen::draw() {
    updateSettings();
    tft->fillScreen(BLACK);
    drawBackButton();
    tft->setFont(&FreeSansBold24pt7b);
    tft->setTextColor(WHITE, BLACK);
    tft->setCursor(100, 50);
    tft->println("Alarms");
    drawSetButton("Pmax", "Alarm", String(settings.peakAlarm),  BLACK, RED, 4, 70);
    drawSetButton("MinVHi", "Alarm", String(settings.mvhiAlarm), BLACK, RED, 123, 70);
    drawSetButton("MinVLo", "Alarm", String(settings.mvloAlarm), BLACK, RED, 242, 70);
    drawSetButton("Disc", "Alarm", String(settings.dcAlarm), BLACK, RED, 361, 70);
};

void AlarmScreen::drawSetButton(String label, String label2, String value, unsigned int fcolor, unsigned int bcolor, int x, int y) {
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

void AlarmScreen::drawBackButton() {
    tft->fillRect(361, 4, 115, 62, GREY);
    tft->fillTriangle(361, 66, 335, 35, 361, 4, GREY);
    tft->setTextSize(1);
    tft->setFont(&FreeSansBold12pt7b);
    tft->setTextColor(BLACK, GREY);
    tft->setCursor(380, 45);
    tft->println("BACK");
};

void AlarmScreen::handleTouch(TSPoint p) {
    // xQueuePeek(settingQ, &settings, 10);
    if (p.x > 330 && p.y < 66) { // back button
        xQueueOverwrite(settingQ, &settings);
        *screen = MAINSCREEN;
    } else if (p.x < 120 && p.y > 194) {
    } else if (p.x < 240 && p.y > 194) {
    } else if (p.x < 360 && p.y > 194) {
    } else if (p.y > 194) {
    } else if (p.x < 120 && p.y > 70) {
        *modvals = {"Pmax Alarm", PIP_ALRM_MIN, PIP_ALRM_MAX, settings.peakAlarm, settings.peakAlarm};
        *screen = MODSCREEN;
    } else if (p.x < 240 && p.y > 70) {
        *modvals = {"MinVHi Alarm", MVHI_ALRM_MIN, MVHI_ALRM_MAX, settings.mvhiAlarm, settings.mvhiAlarm};
        *screen = MODSCREEN;
    } else if (p.x < 360 && p.y > 70) {
        *modvals = {"MinVLo Alarm", MVLO_ALRM_MIN, MVLO_ALRM_MAX, settings.mvloAlarm, settings.mvloAlarm};
        *screen = MODSCREEN;
    } else if (p.y > 70) {
        *modvals = {"DC Alarm", DC_ALRM_MIN, DC_ALRM_MAX, settings.dcAlarm, settings.dcAlarm};
        *screen = MODSCREEN;
    } 
}

void AlarmScreen::updateSettings() {
    if (modvals->newval != modvals->val) {
        if (modvals->label == "Pmax Alarm") {
            settings.peakAlarm = modvals->newval;
        } else if (modvals->label == "MinVHi Alarm") {
            settings.mvhiAlarm = modvals->newval;
        } else if (modvals->label == "MinVLo Alarm") {
            settings.mvloAlarm = modvals->newval;
        } else if (modvals->label == "DC Alarm") {
            settings.dcAlarm = modvals->newval;
        } 
    }
}