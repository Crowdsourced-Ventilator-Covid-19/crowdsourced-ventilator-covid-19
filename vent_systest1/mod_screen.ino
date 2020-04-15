/*
 * This screen allows you to modify the value of one parameter
 */

void drawModScreen(String label, int minv, int maxv, int val){
  screen = "modify";
  clearScreen();
  measLoop();
  drawBackButton();
  measLoop();
  tft.setTextSize(1);
  tft.setTextColor(WHITE, BLACK);
  tft.setFont(&FreeSansBold24pt7b);
  tft.setCursor(10, 50);
  tft.println(label);
  updateModScreen(minv, maxv, val);
  omodVal = val;
}

void updateModScreen(int minv, int maxv, int val) {
  measLoop();
  if (omodVal == val) { return; }
  tft.setTextColor(WHITE, BLACK);
  tft.setFont();
  tft.setTextSize(5);
  tft.setCursor(170, 170);
  // tft.fillRect(170, 90, 170, 90, BLACK);
  measLoop();
  tft.println(String(val));
  measLoop();
  drawSlider(minv, maxv, val);
  omodVal = val;
  measLoop();

}
/*
void updateModScreen(int minv, int maxv, int val, boolean redraw) {
  measLoop();
  if (omodVal == val && ~redraw) { return; }
  tft.setTextColor(WHITE, BLACK);
  tft.setCursor(170, 170);
  tft.setTextSize(2);
  tft.fillRect(170, 90, 170, 90, BLACK);
  measLoop();
  tft.setFont(&FreeSansBold24pt7b);
  tft.println(String(val));
  measLoop();
  drawSlider(minv, maxv, val);
  omodVal = val;
  measLoop();

}
*/
void drawSlider(int minv, int maxv, int val) {
  tft.fillRect(20, 250, 460, 290, BLACK); 
  int xpos = round( (double) (val - minv) / (double) (maxv - minv) * 400 + 20);
  tft.drawLine(40, 270, 440, 270, WHITE);
  tft.fillRoundRect(xpos, 250, 40, 40, 5, GREEN);
}
