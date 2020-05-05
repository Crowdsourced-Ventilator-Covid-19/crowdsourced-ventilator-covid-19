void drawMainScreen() {
  screen = "main";
  double x, y;
  display1 = 2;
  display2 = 2;
  display3 = 2;
  clearScreen();
  tft.setFont();
  tft.setTextSize(1);
  
  drawMeas("Ppeak cmh2o", 380, 380, 10, String(round(peak)), YELLOW);
  drawMeas("Pplat cmh2o", 380, 380, 60, String(round(plat)), YELLOW);
  drawMeas("PEEP cmh2o", 380, 380, 110, String(round(peep)), YELLOW);
  drawMeas("RR bpm", 380, 380, 160, String(fifoMv.rrMeas), WHITE);
  drawMeas("TV ml", 380, 380, 210, String(round(tvMeas)), GREEN);
  drawMeas("MV lpm", 380, 380, 260, String(fifoMv.minvol / 1000.0), GREEN);
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
    display1 = 2;
    display2 = 2;
    display3 = 2;
  }
  measLoop();
  Graph(tft, tmpP_t, tmpP, 40, 90, 320, 80, 0, 15, 1, -10, 50, 10, "Pressure", "", "cmH2o", DKBLUE, RED, YELLOW, WHITE, BLACK, display1, &ox1, &oy1, xticks1 );
  measLoop();
  Graph(tft, tmpF_t, tmpTv, 40, 190, 320, 80, 0, 15, 1, 0, 600, 100, "Volume", "", "cc", DKBLUE, RED, GREEN, WHITE, BLACK, display2, &ox2, &oy2, xticks2);
  measLoop();
  Graph(tft, tmpF_t, tmpF, 40, 290, 320, 85, 0, 15, 1, -60, 60, 20, "Flow", "", "lpm", DKBLUE, RED, WHITE, WHITE, BLACK, display3, &ox3, &oy3, xticks3);
  measLoop();
  last_t = t;
}

// update numbers on main screen
void updateMeasures() {
  boolean xticks1 = false;
  boolean xticks2 = false;
  boolean xticks3 = true;
  if (screen != "main") { return; }
  updateMeas(380, 10, String(round(peak)), YELLOW);
  updateMeas(380, 60, String(round(plat)), YELLOW);
  updateMeas(380, 110, String(round(peep)), YELLOW);
  updateMeas(380, 160, String(fifoMv.rrMeas), WHITE);
  updateMeas(380, 210, String(round(tvMeas)), GREEN);
  updateMeas(380, 260, String(fifoMv.minvol / 1000.0), GREEN);
}

// draw measurement info
void drawMeas(String label, int labelx, int valuex, int y, String value, unsigned int vcolor) {
  tft.setFont();
  tft.setTextSize(1);
  tft.setTextColor(WHITE, BLACK);
  tft.setCursor(labelx, y);
  tft.println(label);
  updateMeas(valuex, y, value, vcolor);
}
// update the measurement number
void updateMeas(int x, int y, String value, unsigned int vcolor) {
  tft.setTextSize(3);
  tft.setFont();
  tft.setTextColor(vcolor, BLACK);
  tft.setCursor(x, y + 15);
  tft.println("   ");
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
