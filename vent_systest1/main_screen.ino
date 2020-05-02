void drawMainScreen() {
  screen = "main";
  double x, y;
  display1 = 2;
  display2 = 2;
  display3 = 2;
  //tft.fillScreen(BLACK);
  clearScreen();
  tft.setFont();
  tft.setTextSize(1);
  
  drawMeas("Ppeak", 380, 400, 10, peak, YELLOW);
  drawMeas("Pplat", 380, 400, 70, plat, YELLOW);
  drawMeas("PEEP", 380, 400, 130, peep, YELLOW);
  drawMeas("RR", 380, 400, 190, rr, WHITE);
  drawMeas("TV", 380, 390, 250, tvMeas, GREEN);
}
/*
 * This screen shows the graphs, and measured values
 */


void updateGraphs() {
  if (screen != "main") { return; }
  boolean xticks1 = false;
  boolean xticks2 = false;
  boolean xticks3 = true;
  
  double t = (millis() % 15000) / 1000.0;
  if (t < last_t) {
    display1 = 1;
    display2 = 1;
    display3 = 1;
    // tft.fillRect(1, 0, 360, 320, BLACK);
  }
  measLoop();
  Graph(tft, (psens.t % 15000) / 1000.0, psens.p, 40, 90, 320, 80, 0, 15, 1, -10, 50, 10, "Pressure", "", "cmH2o", DKBLUE, RED, YELLOW, WHITE, BLACK, display1, &ox1, &oy1, xticks1 );
  measLoop();
  Graph(tft, (fsens.t % 15000) / 1000.0, fsens.tv, 40, 190, 320, 80, 0, 15, 1, 0, 600, 100, "Volume", "", "cc", DKBLUE, RED, GREEN, WHITE, BLACK, display2, &ox2, &oy2, xticks2);
  measLoop();
  Graph(tft, (fsens.t % 15000) / 1000.0, fsens.f, 40, 290, 320, 85, 0, 15, 1, -60, 60, 20, "Flow", "", "lpm", DKBLUE, RED, WHITE, WHITE, BLACK, display3, &ox3, &oy3, xticks3);
  measLoop();
  last_t = t;
}

// update numbers on main screen
void updateMeasures() {
  boolean xticks1 = false;
  boolean xticks2 = false;
  boolean xticks3 = true;
  if (screen != "main") { return; }
  updateMeas(400, 10, peak, YELLOW);
  updateMeas(400, 70, plat, YELLOW);
  updateMeas(400, 130, peep, YELLOW);
  updateMeas(390, 250, tvMeas, GREEN);
}

// draw measurement info
void drawMeas(String label, int labelx, int valuex, int y, int value, unsigned int vcolor) {
  tft.setFont();
  tft.setTextSize(1);
  tft.setTextColor(WHITE, BLACK);
  tft.setCursor(labelx, y);
  tft.println(label);
  updateMeas(valuex, y, value, vcolor);
}
// update the measurement number
void updateMeas(int x, int y, int value, unsigned int vcolor) {
  tft.setTextSize(5);
  tft.setFont();
  tft.setTextColor(vcolor, BLACK);
  tft.setCursor(x, y + 15);
  tft.println(value);
}
/*
// update the measurement number
void updateMeas(int x, int y, int value, unsigned int vcolor) {
  tft.fillRect(x, y + 15, 100, 40, BLACK);
  tft.setTextSize(1);
  tft.setFont(&FreeSansBold24pt7b);
  tft.setTextColor(vcolor, BLACK);
  tft.setCursor(x, y + 50);
  tft.println(value);
}
/*
