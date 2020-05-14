#include "mod_screen.h"
#include <Adafruit_HX8357.h>
#include <TouchScreen.h>

#define BLACK     0x0000
#define GREY      0xC618
#define WHITE     0xFFFF
#define RED       0xF800
#define YELLOW    0xFFE0
#define GREEN     0x07E0

ModScreen::ModScreen(Adafruit_HX8357 &tft, QueueHandle_t screenQ, QueueHandle_t settingQ) {
    this->tft = &tft;
    this->settingQ = settingQ;
    this->screenQ = screenQ;
};

void ModScreen::draw(String label, int minv, int maxv, int val, int scale) {
    tft->fillScreen(BLACK);
    drawBackButton();
    tft->setTextSize(1);
    tft->setTextColor(WHITE, BLACK);
    tft->setFont(&FreeSansBold24pt7b);
    tft->setCursor(10, 50);
    tft->println(label);
};

void ModScreen::handleTouch(TSPoint p) {
    if (p.x > 330 && p.y < 66) { // back button
        screen = SETSCREEN;
        xQueueOverwrite(screenQ, &screen);
    }

    if (p.x < 40) { p.x = 40; }
    if (p.x > 440) { p.x = 440; }
    val = round(float(p.x-40) / 400.0 * float(maxv - minv)) + minv;
    if (oval != val) {
        tft->setTextColor(WHITE, BLACK);
        tft->setFont();
        tft->setTextSize(5);
        tft->setCursor(170, 170);
        tft->println("     ");
        tft->setCursor(170, 170);
        tft->println(String(val));
        drawSlider(minv, maxv, val);
        oval = val;
    }

}

void ModScreen::drawSlider(int minv, int maxv, int val) {
  tft->fillRect(20, 250, 460, 290, BLACK); 
  int xpos = round( (double) (val - minv) / (double) (maxv - minv) * 400 + 20);
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