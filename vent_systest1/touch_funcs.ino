/*
 * Touch handler
 */

#define MINPRESSURE 200
#define MAXPRESSURE 1000

String modVar;              // variable being modified
int    modVal;              // placeholder for new variable


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
      tvoff = 0;
      } else if (xpos < 120 && ypos > 194) {
        modVar = "trig";
        modVal = trig;
        drawModScreen("AC Trig", "modify", TRIG_MIN, TRIG_MAX, modVal);
      } else if (xpos < 240 && ypos > 194) {
        power = !power;
        String powertxt = (power)? "STOP" : "RUN ";
        tft.fillRect(123,220,120,100,BLACK);
        drawSetButton("Power", "", powertxt, BLACK, WHITE, 123, 194);
      } else if (xpos < 360 && ypos > 194) {

      } else if (ypos > 194) {
        drawAlarmScreen();
      } else if (xpos < 120 && ypos > 70) {
        modVar = "rr";
        modVal = rr;
        drawModScreen("RR", "modify", RR_MIN, RR_MAX, modVal);
      } else if (xpos < 240 && ypos > 70) {
        modVar = "tv";
        modVal = tvSet;
        drawModScreen("TV", "modify", TV_MIN, TV_MAX, modVal);
      } else if (xpos < 360 && ypos > 70) {
        modVar = "ier";
        modVal = ier;
        drawModScreen("I/E Ratio", "modify", IE_MIN, IE_MAX, modVal);
      } else if (ypos > 70) {
        modVar = "pmax";
        modVal = pmax;
        drawModScreen("Pmax", "modify", PIP_MIN, PIP_MAX, modVal);
      }
    } 
    else if (screen == "alarm") {
      if (xpos > 330 && ypos < 66) { // back button
      display1 = 2;
      display2 = 2;
      display3 = 2;
      drawMainScreen(); 
      } else if (xpos < 120 && ypos > 194) {

      } else if (xpos < 240 && ypos > 194) {

        
      } else if (xpos < 360 && ypos > 194) {

      } else if (ypos > 194) {
        
      } else if (xpos < 120 && ypos > 70) {
        modVar = "peakAlrm";
        modVal = peakAlrm;
        drawModScreen("Ppeak Alarm", "modifyAlarm", PIP_ALRM_MIN, PIP_ALRM_MAX, modVal);
      } else if (xpos < 240 && ypos > 70) {
        modVar = "mvhiAlrm";
        modVal = mvhiAlrm;
        drawModScreen("MinVolHi Alarm", "modifyAlarm", MVHI_ALRM_MIN, MVHI_ALRM_MAX, modVal);
      } else if (xpos < 360 && ypos > 70) {
        modVar = "mvloAlrm";
        modVal = mvloAlrm;
        drawModScreen("MinVolLo Alarm", "modifyAlarm", MVLO_ALRM_MIN, MVLO_ALRM_MAX, modVal);
      } else if (ypos > 70) {
        modVar = "dcAlrm";
        modVal = dcAlrm;
        drawModScreen("Disconnect Alarm", "modifyAlarm", DC_ALRM_MIN, DC_ALRM_MAX, modVal);
      }
    } 
    else if (screen == "modify") {
      if (xpos > 330 && ypos < 66) { // back button
        if (modVar == "trig") {trig = modVal;}
        else if (modVar == "rr") {
          rr = modVal;
          fifoMv.fifoInit(rr,tvSet);
        }
        else if (modVar == "tv") {tvSet = modVal;}
        else if (modVar == "ier") {ier = modVal;}
        else if (modVar == "pmax") {pmax = modVal;}
        omodVal = -999;
        drawSetScreen(); 
      } else {
        if (xpos < 40) { xpos = 40; }
        if (xpos > 440) { xpos = 440; }
        if (modVar == "trig") { modVal = updateModScreen(TRIG_MIN, TRIG_MAX, xpos);}
        else if (modVar == "rr") { modVal = updateModScreen(RR_MIN, RR_MAX, xpos);}
        else if (modVar == "tv") { modVal = updateModScreen(TV_MIN, TV_MAX, xpos);}
        else if (modVar == "ier") { modVal = updateModScreen(IE_MIN, IE_MAX, xpos);}
        else if (modVar == "pmax") { modVal = updateModScreen(PIP_MIN, PIP_MAX, xpos);}
      }
    }
    else if (screen == "modifyAlarm") {
      if (xpos > 330 && ypos < 66) { // back button
        if (modVar == "peakAlrm") { peakAlrm = modVal; }
        else if (modVar == "mvhiAlrm") {mvhiAlrm = modVal;}
        else if (modVar == "mvloAlrm") {mvloAlrm = modVal;}
        else if (modVar == "dcAlrm") {dcAlrm = modVal;}
        omodVal = -999;
        drawAlarmScreen(); 
      } else {
        if (xpos < 40) { xpos = 40; }
        if (xpos > 440) { xpos = 440; }
        if (modVar == "peakAlrm") { modVal = updateModScreen(PIP_ALRM_MIN, PIP_ALRM_MAX, xpos);}
        else if (modVar == "mvhiAlrm") { modVal = updateModScreen(MVHI_ALRM_MIN, MVHI_ALRM_MAX, xpos);}
        else if (modVar == "mvloAlrm") { modVal = updateModScreen(MVLO_ALRM_MIN, MVLO_ALRM_MAX, xpos);}
        else if (modVar == "dcAlrm") { modVal = updateModScreen(DC_ALRM_MIN, DC_ALRM_MAX, xpos);}
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
