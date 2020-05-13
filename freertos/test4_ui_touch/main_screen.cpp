#include "main_screen.h"
#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_HX8357.h>
#include "graph.h"

#define BLACK     0x0000
#define GREY      0xC618
#define WHITE     0xFFFF
#define RED       0xF800
#define YELLOW    0xFFE0
#define GREEN     0x07E0
#define DKBLUE    0x000D

MainScreen::MainScreen(Adafruit_HX8357 &tft) {
    this->tft = &tft;
    pGraph = Graph(tft, 40, 90, 320, 80, 0, 15, 1, -10, 50, 10, "Pressure", "", "cmH2o", DKBLUE, RED, YELLOW, WHITE, BLACK);
    fGraph = Graph(tft, 40, 190, 320, 80, 0, 15, 1, 0, 800, 200, "Volume", "", "ml", DKBLUE, RED, GREEN, WHITE, BLACK);
    vGraph = Graph(tft, 40, 290, 320, 85, 0, 15, 1, -60, 60, 20, "Flow", "", "lpm", DKBLUE, RED, WHITE, WHITE, BLACK);
};

void MainScreen::draw() {
    tft->fillScreen(BLACK);
    pGraph.draw();
    fGraph.draw();
    vGraph.draw();
}

void MainScreen::update(Sample_t sample) {
    double t = float(sample.t % 15000) / 1000.0;
    pGraph.plot(sample.t, cos(t) * 10 + 20);
    fGraph.plot(sample.t, sin(t) * 200 + 300);
    vGraph.plot(sample.t, cos(t) * 40);
}