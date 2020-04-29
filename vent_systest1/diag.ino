#define TCAADDR 0x70

void diag() {
  screen = "diag";
  tft.fillScreen(BLACK);
  tft.setFont();
  tft.setTextSize(1);
  tft.setTextColor(WHITE, BLACK);
  
  tcascan();

  tft.println("Initializing Sensors...");
  initAMS(ams0, 0);
  initAMS(ams1, 1);
    
  // tare differential pressure sensors
  calibratePressure(ams1,1);
  calibrateFlow(ams0, 0);
  
  delay(5000);
  
}

void initAMS(AMS5915 &ams, int tca) {
  double t;
  tft.println("Initialize AMS5915 on tca " + String(tca) + "...");
  ams.getAddr_AMS5915(AMS5915_DEFAULT_ADDRESS);   // 0x28
  ams.begin();
  t = readFlow(ams, tca);
  while(isnan(readFlow(ams, tca))) {
    tft.println("Failed to read, retrying");
    delay(100);
  }
  tft.println("Success");
}
