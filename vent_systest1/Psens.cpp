#include "Arduino.h"
#include "Psens.h"
#include "Wire.h"
#define TCAADDR 0x70
#define Addr 0x28

Psens::Psens(int i) {
  tca = i;
  this->tcaselect(tca);
  poff = float(0);
  this->read();
  poff = p;
}

void Psens::read() {
  this->tcaselect(tca);
  unsigned int data[4];
  Wire.requestFrom(Addr, 4);
  if (Wire.available() == 4) {
    data[0] = Wire.read();
    data[1] = Wire.read();
    data[2] = Wire.read();
    data[3] = Wire.read();
  }
  
  float pressure = ((data[0] & 0x3F) * 256.0 + data[1]);
  p = pressure * 1.01972  - poff;
  t = millis();
}

void Psens::tcaselect(int i) {
  if (i > 7) return;
  Wire.beginTransmission(TCAADDR);
  Wire.write(1 << i);
  Wire.endTransmission();
}
