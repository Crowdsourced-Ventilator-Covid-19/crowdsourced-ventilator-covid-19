/*
 * This draws the settings screen which shows the current value of all configurable parameters
 */

void drawSetScreen() {
  screen = "settings";
  tft.fillScreen(BLACK);
  drawBackButton();
  tft.setFont(&FreeSansBold24pt7b);
  tft.setTextColor(WHITE, BLACK);
  tft.setCursor(100, 50);
  tft.println("Settings");
  drawSetButton("RR", "", String(rr),  BLACK, WHITE, 4, 70);
  drawSetButton("TV", "", String(tvSet), BLACK, GREEN, 123, 70);
  drawSetButton("I/E", "", String("1:" + String(ier)), BLACK, WHITE, 242, 70);
  drawSetButton("Pmax", "", String(pmax), BLACK, YELLOW, 361, 70);
  drawSetButton("Ppeak", "Alarm", String(peakAlrm), BLACK, RED, 4, 194);
  drawSetButton("Pplat", "Alarm", String(platAlrm), BLACK, RED, 123, 194);
  drawSetButton("PEEP", "Alarm", String(peepAlrm), BLACK, RED, 242, 194);
  drawSetButton("Neg", "Trig", String(negTrig), BLACK, YELLOW, 361, 194);
}

void drawSetButton(String label, String label2, String value, unsigned int fcolor, unsigned int bcolor, double x, double y) {
  tft.drawRoundRect(x, y, 115, 120, 7, bcolor);
  // tft.fillRoundRect(x, y, 115, 120, 7, bcolor);
  tft.setTextSize(1);
  tft.setFont(&FreeSans12pt7b);
  // tft.setTextColor(fcolor, bcolor);
  tft.setTextColor(bcolor, fcolor);
  tft.setCursor(x + 20, y + 30);
  tft.println(label);
  tft.setCursor(x + 20, y + 60);
  tft.println(label2);
  tft.setFont(&FreeSansBold24pt7b);
  tft.setCursor(x+10, y+110);
  tft.println(value);
}

void drawBackButton() {
  tft.fillRect(361, 4, 115, 62, GREY);
  tft.fillTriangle(361, 66, 335, 35, 361, 4, GREY);
  tft.setTextSize(1);
  tft.setFont(&FreeSansBold12pt7b);
  tft.setTextColor(BLACK, GREY);
  tft.setCursor(380, 45);
  tft.println("BACK");
}
