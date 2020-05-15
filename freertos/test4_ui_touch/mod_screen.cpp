#include "mod_screen.h"
#include <Adafruit_HX8357.h>
#include <TouchScreen.h>

#define BLACK     0x0000
#define GREY      0xC618
#define WHITE     0xFFFF
#define RED       0xF800
#define YELLOW    0xFFE0
#define GREEN     0x07E0

ModScreen::ModScreen(Adafruit_HX8357 &tft, Screen &screen, ModVal_t &modvals) {
    this->tft = &tft;
    this->settingQ = settingQ;
    this->screen = &screen;
    this->modvals = &modvals;
};

void ModScreen::draw() {
    oval = modvals->val;
    val = modvals->val;
    tft->fillScreen(BLACK);
    drawBackButton();
    tft->setTextSize(1);
    tft->setTextColor(WHITE, BLACK);
    tft->setFont(&FreeSansBold24pt7b);
    tft->setCursor(10, 50);
    tft->println(modvals->label);
    drawValue();
    drawSlider();
};

void ModScreen::handleTouch(TSPoint p) {
    if (p.x > 330 && p.y < 66) { // back button
        modvals->newval = val;
        *screen = SETSCREEN;
    } else {
        if (p.x < 40) { p.x = 40; }
        if (p.x > 440) { p.x = 440; }
        val = map(p.x, 40, 440, modvals->minv, modvals->maxv);
        if (oval != val) {
            drawValue();
            drawSlider();
            oval = val;
        }
    }
}

void ModScreen::drawValue() {
    tft->setTextColor(WHITE, BLACK);
    tft->setFont(&FreeSansBold24pt7b);
    tft->setTextSize(1);
    tft->fillRect(170,120,120,60,BLACK);
    tft->setCursor(170, 170);
    if (modvals->label == "I/E Ratio") {
        char buffer[5];
        sprintf(buffer, "1:%2.1f", float(val) / 10.0);
        tft->println(buffer);
    } else {
        tft->println(String(val));
    }
}

void ModScreen::drawSlider() {
  tft->fillRect(20, 250, 460, 290, BLACK); 
  int xpos = map(val, modvals->minv, modvals->maxv, 20, 420);
  tft->drawLine(40, 270, 440, 270, WHITE);
  tft->fillRoundRect(xpos, 250, 40, 40, 5, GREEN);
}

void ModScreen::drawBackButton() {
    tft->fillRect(361, 4, 115, 62, GREY);
    tft->fillTriangle(361, 66, 335, 35, 361, 4, GREY);
    tft->setTextSize(1);
    tft->setFont(&FreeSansBold12pt7b);
    tft->setTextColor(BLACK, GREY);
    tft->setCursor(380, 45);
    tft->println("BACK");
};