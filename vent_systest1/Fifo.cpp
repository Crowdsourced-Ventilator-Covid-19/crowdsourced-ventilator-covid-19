#include "Arduino.h"
#include "Fifo.h"


// initialize the fifo values to zero
Fifo::Fifo(int d, double n) {
  depth = d;
  for (int idx = 0; idx < depth; idx++) {
    entries[idx] = 0;
  }
  idx = 0;
  total = 0;
  last_t = millis();
  last_v = 0;
  noise = n;
  peak = 0;
}

void Fifo::fifoPush(double v) {
  total = total - entries[idx];
  entries[idx] = v;
  total = total + v;
  idx = idx + 1;
  if (idx >= depth) { idx = 0; }
  avg = total / (double) depth;
  if (v > peak) { peak = v; }
}

void Fifo::fifoPushDeriv(double v, uint32_t t) {
  double slope = abs((v - last_v) / (t - last_t));
  double avgSlope = abs(total / (double) depth);
  last_v = v;
  last_t = t;
  total = total - entries[idx];
  entries[idx] = slope;
  idx = idx + 1;
  if (idx >= depth) { idx = 0; }
  converging =  (slope <= avgSlope + noise);
}
