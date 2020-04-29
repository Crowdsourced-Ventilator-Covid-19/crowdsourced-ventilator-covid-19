const float AMS5915_0100_D_MIN = 0.0;       // minimum pressure, millibar
const float AMS5915_0100_D_MAX = 100.0;     // maximum pressure, millibar

/*
 * This is a dummy placeholder for taking a pressure or flow measurement
 */
// read patient and reference pressure sensors to calculate guage pressure in cmh2o
double readPressure(AMS5915 &ams, int tca) {
  float p_cmh2o;
  tcaselect(tca);
  ams.Measure_PressureAndTemperature(AMS5915_0100_D_MIN, AMS5915_0100_D_MAX);
  p_cmh2o = ams.getPressure() * 1.01972;
  return p_cmh2o; 
}

// read quiescent pressure value and store it as zero offset
void calibratePressure(AMS5915 &ams, int tca) {
  double tmp;
  tft.println("Calibrating AMS pressure on tca " + String(tca) + "...");
  poff = 0;
  tmp = readPressure(ams, tca);
  while(isnan(tmp)) {
    tft.println("Failed to read, retrying");
    delay(500);
    tmp = readPressure(ams, tca);
  }
  poff = tmp;
  tft.println("Success Poff = " + String(poff));
}


// read the venturi flow meter and return value in lpm
double readFlow(AMS5915 &ams, int tca) {
  float r1, r2, A1, A2, rho, deltaP, flow;
  tcaselect(tca);     // select channel
  ams.Measure_PressureAndTemperature(AMS5915_0100_D_MIN, AMS5915_0100_D_MAX);

  // venturi flow meter constants
  r1 = 0.0095;        // outer radius (m)
  r2 = 0.00189;       // inner radius (m)
  A1 = PI * sq(r1);   // outer area (m^2)
  A2 = PI * sq(r2);   // inner area (m^2)
  rho = 1.225;        // density of air @ sea level (kg/m^3)

  // Flow calculation from https://www.engineeringtoolbox.com/orifice-nozzle-venturi-d_590.html
  deltaP = ams.getPressure() * 100.0; // mbar to N/m2
  if (deltaP < 0) { deltaP = 0; } // dont measure negative flow
  
  flow = A2 * sqrt(2 * deltaP / rho / (1 - sq(A2/A1))) * 1000 * 60 - foff;

  // If flow is below flowDeadZone in magnitude, ignore it to avoid integration error
  if (abs(flow) < flowDeadZone) { flow = 0; }
  return flow;
}

// read quiescent flow value, and store it as zero offset
void calibrateFlow(AMS5915 &ams, int tca) {
  tcaselect(tca);
  double tmp;
  tft.println("Calibrating AMS Flow on tca " + String(tca) + "...");
  foff = 0;
  tmp = readFlow(ams, tca);
  while(isnan(tmp)) {
    tft.println("Failed to read, retrying");
    delay(500);
    tmp = readFlow(ams, tca);
  }
  foff = tmp;
  tft.println("Success foff = " + String(foff));
}
