/*
 * This is a dummy placeholder for taking a pressure or flow measurement
 */
// read patient and reference pressure sensors to calculate guage pressure in cmh2o
double readPressure() {
  if (millis() > pAtmosTimer || p_atmos_hPa == 0) {
    // read atmospheric reference
    tcaselect(1);
    p_atmos_hPa = mpr.readPressure(); 
    pAtmosTimer = millis() + pAtmosDelay;
  }
  
  // read pressure sensor in hectoPa
  tcaselect(0);
  double p_hPa = mpr.readPressure();
  
  // convert to relative pressure in cmh2o
  return (p_hPa - p_atmos_hPa) * 1.01974428892 - poff;
}

// read quiescent pressure value and store it as zero offset
void calibratePressure() {
  double tmp;
  tft.println("Calibrating pressure zero...");
  poff = 0;
  tmp = readPressure();
  while(isnan(tmp)) {
    tft.println("Failed to read, retrying");
    delay(500);
    tmp = readPressure();
  }
  poff = tmp;
  tft.println("Success");
}

const float AMS5915_0100_D_MIN = 0.0;       // minimum pressure, millibar
const float AMS5915_0100_D_MAX = 100.0;     // maximum pressure, millibar

// read the venturi flow meter and return value in lpm
double readFlow() {
  float r1, r2, A1, A2, rho, deltaP, flow;
  tcaselect(0);       // flow meter on channel 0
  ams.Measure_PressureAndTemperature(AMS5915_0100_D_MIN, AMS5915_0100_D_MAX);

  // venturi flow meter constants
  r1 = 0.0095;        // outer radius (m)
  r2 = 0.00189;       // inner radius (m)
  A1 = PI * sq(r1);   // outer area (m^2)
  A2 = PI * sq(r2);   // inner area (m^2)
  rho = 1.225;        // density of air @ sea level (kg/m^3)

  // Flow calculation from https://www.engineeringtoolbox.com/orifice-nozzle-venturi-d_590.html
  deltaP = ams.getPressure() * 100.0; // mbar to N/m2
  flow = A2 * sqrt(2 * deltaP / rho * (1 - sq(A2/A1))) * 1000 * 60 - foff;
  
  // If flow is below flowDeadZone in magnitude, ignore it to avoid integration error
  if (abs(flow) < flowDeadZone) { flow = 0; }
  return flow;
}

// read quiescent flow value, and store it as zero offset
void calibrateFlow() {
  double tmp;
  tft.println("Calibrating AMS zero...");
  foff = 0;
  tmp = readFlow();
  while(isnan(tmp)) {
    tft.println("Failed to read, retrying");
    delay(500);
    tmp = readFlow();
  }
  foff = tmp;
  tft.println("Success");
}
