#include "Arduino.h"
#include "Fifo.h"


// initialize the fifo values to zero
Fifo::Fifo(int d) {
  depth = d;
  for (int idx = 0; idx < depth; idx++) {
    entries[idx] = 0;
  }
  idx = 0;
  total = 0;
  last_t = millis();
  last_v = 0;
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

void Fifo::fifoInit() {
  for (int idx = 0; idx < depth; idx++) {
    entries[idx] = 0;
  }
  total = 0;
}
