void drawAlarmScreen() {
  screen = "alarm";
  clearScreen();
  measLoop();
  drawBackButton();
  measLoop();
  tft.setFont(&FreeSansBold24pt7b);
  tft.setTextColor(WHITE, BLACK);
  tft.setCursor(100, 50);
  tft.println("Alarms");
  drawSetButton("Pmax", "", String(peakAlrm),  BLACK, RED, 4, 70);
  drawSetButton("MVHi", "", String(mvhiAlrm), BLACK, RED, 123, 70);
  drawSetButton("MVLo", "", String(mvloAlrm), BLACK, RED, 242, 70);
  drawSetButton("DC", "", String(pmax), BLACK, RED, 361, 70);
}
