#define TCAADDR 0x70

void diag() {
  screen = "diag";
  tft.fillScreen(BLACK);
  tft.setFont();
  tft.setTextSize(1);
  tft.setTextColor(WHITE, BLACK);
  
  tcascan();

  tft.println("Initializing Sensors...");
  initAMS(0);
  initMPRLS(1);
  initMPRLS(0);
  
  // tare differential pressure sensors
  calibratePressure();
  calibrateFlow();
  
  delay(5000);
  
}

void initAMS(int tca) {
  double t;
  tft.println("Initialize AMS5915 on tca " + String(tca) + "...");
  tcaselect(tca);
  ams.getAddr_AMS5915(AMS5915_DEFAULT_ADDRESS);   // 0x28
  ams.begin();
  t = readFlow();
  while(isnan(readFlow())) {
    tft.println("Failed to read, retrying");
    delay(100);
  }
  tft.println("Success");
}
void initMPRLS(int tca) {
  double t;
  tft.println("Initialize MPRLS on tca " + String(tca) + "...");
  tcaselect(tca);
  while (!mpr.begin()) {
    tft.println("Failed to init, retrying");
    delay(100);
  }
  while(isnan(readPressure())) {
    tft.println("Failed to read, retrying");
    delay(100);
  }
  tft.println("Success");
}
