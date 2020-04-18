/*
 * Touch handler
 */

void checkTouch() {
  uint16_t xpos,ypos; // screen coords
  int minv,maxv; // slider vars
  tp = ts.getPoint(); //tp.x, tp.y are ADC values
  
  // if sharing pins, you'll need to fix the directions of the touchscreen pins
  pinMode(XM, OUTPUT);
  pinMode(YP, OUTPUT);

  // we have some minimum pressure we consider 'valid'
  // pressure of 0 means no pressing!
  if (tp.z > MINPRESSURE && tp.z < MAXPRESSURE) {
    xpos = map(tp.y, TS_TOP, TS_BOT, 0, tft.width());
    ypos = map(tp.x, TS_RT, TS_LEFT, 0, tft.height());


    if (screen == "main") {
      drawSetScreen();
    } 
    else if (screen == "settings") {
      if (xpos > 330 && ypos < 66) { // back button
      display1 = 2;
      display2 = 2;
      display3 = 2;
      drawMainScreen(); 
      } else if (xpos < 120 && ypos > 194) {
        modVar = "peakAlrm";
        modVal = peakAlrm;
        drawModScreen("Ppeak Alarm", 10, 40, modVal);
      } else if (xpos < 240 && ypos > 194) {
        modVar = "platAlrm";
        modVal = platAlrm;
        drawModScreen("Pplat Alarm", 10, 40, modVal);
      } else if (xpos < 360 && ypos > 194) {
        modVar = "peepAlrm";
        modVal = peepAlrm;
        drawModScreen("PEEP Alarm", 0, 20, modVal);
      } else if (ypos > 194) {
        modVar = "negTrig";
        modVal = negTrig;
        drawModScreen("Neg Trig", -10, -1, modVal);
      } else if (xpos < 120 && ypos > 70) {
        modVar = "rr";
        modVal = rr;
        drawModScreen("RR", 10, 30, modVal);
      } else if (xpos < 240 && ypos > 70) {
        modVar = "tv";
        modVal = tvSet;
        drawModScreen("TV", 100, 600, modVal);
      } else if (xpos < 360 && ypos > 70) {
        modVar = "ier";
        modVal = ier;
        drawModScreen("I/E Ratio", 1, 3, modVal);
      } else if (ypos > 70) {
        modVar = "pmax";
        modVal = pmax;
        drawModScreen("Pmax", 10, 40, modVal);
      }
    } 
    
    else if (screen == "modify") {
      if (xpos > 330 && ypos < 66) { // back button
        if (modVar == "peakAlrm") { peakAlrm = modVal; }
        else if (modVar == "platAlrm") {platAlrm = modVal;}
        else if (modVar == "peepAlrm") {peepAlrm = modVal;}
        else if (modVar == "negTrig") {negTrig = modVal;}
        else if (modVar == "rr") {rr = modVal;}
        else if (modVar == "tv") {tvSet = modVal;}
        else if (modVar == "ier") {ier = modVal;}
        else if (modVar == "pmax") {pmax = modVal;}
        omodVal = -999;
        drawSetScreen(); 
      } else {
        if (xpos < 40) { xpos = 40; }
        if (xpos > 440) { xpos = 440; }
        if (modVar == "peakAlrm") { updateModScreen(10, 40, getModVal(10, 40, xpos));}
        else if (modVar == "platAlrm") { updateModScreen(10, 40, getModVal(10, 40, xpos));}
        else if (modVar == "peepAlrm") { updateModScreen(0, 20, getModVal(0, 20, xpos));}
        else if (modVar == "negTrig") { updateModScreen(-10, -1, getModVal(-10, -1, xpos));}
        else if (modVar == "rr") { updateModScreen(10, 30, getModVal(10, 30, xpos));}
        else if (modVar == "tv") { updateModScreen(100, 600, getModVal(100, 600, xpos));}
        else if (modVar == "ier") { updateModScreen(1, 3, getModVal(1, 3, xpos));}
        else if (modVar == "pmax") { updateModScreen(10, 40, getModVal(10, 40, xpos));}
      }
    }
  }
}

int getModVal(int minv, int maxv, int xpos) {
  modVal = round((double) (xpos - 40) / (double) 400 * (double) (maxv - minv)) + minv;
  return modVal;
}

void waitForTouch() {
  tft.setFont();
  tft.setTextSize(1);
  tft.setTextColor(WHITE, BLACK);
  tft.println("");
  tft.println("Tap screen to continue");
  tp = ts.getPoint(); //tp.x, tp.y are ADC values
  while (tp.z < MINPRESSURE || tp.z > MAXPRESSURE){
    delay(50);
    tp = ts.getPoint();
  }
}
