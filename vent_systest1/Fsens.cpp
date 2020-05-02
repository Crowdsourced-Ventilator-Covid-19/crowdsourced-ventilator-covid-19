#include "Arduino.h"
#include "Fsens.h"
#include "Wire.h"
#include "stdint.h"
#define TCAADDR 0x70
#define Addr 0x28

Fsens::Fsens(int i, double d) {
  tca = i;
  this->tcaselect(tca);
  // AMS5915 ams;
  // ams.getAddr_AMS5915(AMS5915_DEFAULT_ADDRESS); // 0x28
  // ams.begin();
  foff = float(0);
  this->read();
  foff = f;
  deadzone = d;
  ot = millis();
  of = 0;
  
}

void Fsens::read() {
  this->tcaselect(tca);
  unsigned int data[4];
  Wire.requestFrom(Addr, 4);
  if (Wire.available() == 4) {
    data[0] = Wire.read();
    data[1] = Wire.read();
    data[2] = Wire.read();
    data[3] = Wire.read();
  }
  
  // venturi flow meter constants
  const double r1 = 0.0095;        // outer radius (m)
  const double r2 = 0.00189;       // inner radius (m)
  double A1 = PI * sq(r1);   // outer area (m^2)
  double A2 = PI * sq(r2);   // inner area (m^2)
  double rho = 1.225;        // density of air @ sea level (kg/m^3)

  // Flow calculation from https://www.engineeringtoolbox.com/orifice-nozzle-venturi-d_590.html
  float pressure = ((data[0] & 0x3F) * 256.0 + data[1]) * 100.0;  // N/m^2
  flow = A2 * sqrt(2 * pressure / rho / (1 - sq(A2/A1))) * 1000 * 60 - foff;

  // If flow is below flowDeadZone in magnitude, ignore it to avoid integration error
  if (abs(flow) < deadzone) { 
    f = 0; 
  } else { 
    f = flow;
  }
  t = millis();
  uint32_t dt;
  if (ot > t) {
    dt = UINT32_MAX - ot + t;
  } else {
    dt = t - ot;
  }
  tv += float(dt) / 60 * (of + f) / 2;
  ot = t;
  of = f;
}

void Fsens::tcaselect(int i) {
  if (i > 7) return;
  Wire.beginTransmission(TCAADDR);
  Wire.write(1 << i);
  Wire.endTransmission();
}
