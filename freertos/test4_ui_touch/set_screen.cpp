#include "set_screen.h"
#include <Adafruit_HX8357.h>
#include "config.h"

#define BLACK     0x0000
#define GREY      0xC618
#define WHITE     0xFFFF
#define RED       0xF800
#define YELLOW    0xFFE0
#define GREEN     0x07E0

SetScreen::SetScreen(Adafruit_HX8357 &tft, QueueHandle_t screenQ, QueueHandle_t stateQ, QueueHandle_t settingQ) {
    this->tft = &tft;
    this->stateQ = stateQ;
    this->settingQ = settingQ;
    this->screenQ = screenQ;
};

void SetScreen::draw() {
    xQueuePeek(stateQ, &state, 0);
    xQueuePeek(settingQ, &settings, 0);

    tft->fillScreen(BLACK);
    drawBackButton();
    tft->setFont(&FreeSansBold24pt7b);
    tft->setTextColor(WHITE, BLACK);
    tft->setCursor(100, 50);
    tft->println("Settings");
    String powertxt = (state.power)? "STOP" : "RUN ";
    drawSetButton("RR", "", String(settings.rr),  BLACK, WHITE, 4, 70);
    drawSetButton("TV", "", String(settings.tv), BLACK, GREEN, 123, 70);
    drawSetButton("I/E", "", "1:" + String(settings.ier), BLACK, WHITE, 242, 70);
    drawSetButton("Pmax", "", String(settings.pmax), BLACK, YELLOW, 361, 70);
    drawSetButton("AC", "Trig", String(settings.trig), BLACK, YELLOW, 4, 194);
    drawSetButton("Power", "", powertxt, BLACK, WHITE, 123, 194);
    drawSetButton("Alarms", "", String(""), BLACK, RED, 361, 194);
};

void SetScreen::drawSetButton(String label, String label2, String value, unsigned int fcolor, unsigned int bcolor, int x, int y) {
    tft->drawRoundRect(x, y, 115, 120, 7, bcolor);
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
    if (p.x > 330 && p.y < 66) { // back button
        screen = MAINSCREEN;
        xQueueOverwrite(screenQ, &screen);
    } else if (p.x < 120 && p.y > 194) { // trig
        //drawModScreen("AC Trig", "modify", TRIG_MIN, TRIG_MAX, modVal);
    } else if (p.x < 240 && p.y > 194) { // power
       // power = !power;
        //String powertxt = (power)? "STP" : "RUN ";
        //tft.fillRect(123,220,120,100,BLACK);
       // drawSetButton("Power", "", powertxt, BLACK, WHITE, 123, 194);
    } else if (p.x < 360 && p.y > 194) {

    } else if (p.y > 194) {
        
       // drawAlarmScreen();
    } else if (p.x < 120 && p.y > 70) {
       // drawModScreen("RR", "modify", RR_MIN, RR_MAX, modVal);
    } else if (p.x < 240 && p.y > 70) {
        //drawModScreen("TV", "modify", TV_MIN, TV_MAX, modVal);
    } else if (p.x < 360 && p.y > 70) {
       //drawModScreen("I/E Ratio", "modify", IE_MIN, IE_MAX, modVal);
    } else if (p.y > 70) {
        //drawModScreen("Pmax", "modify", PIP_MIN, PIP_MAX, modVal);
    } 
}