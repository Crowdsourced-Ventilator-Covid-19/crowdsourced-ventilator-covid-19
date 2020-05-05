#include "Arduino.h"
#include "Fifo.h"
#include <stdint.h>

// initialize the fifo values to zero
Fifo::Fifo(int d) {
  depth = d;
  for (int idx = 0; idx < depth; idx++) {
    entries[idx] = 0;
  }
  idx = 0;
  total = 0;
  peak = 0;
  minvol = 0;
  rrMeas = d;
}
/*
 * Track pressure average and peak values
 */
void Fifo::fifoPush(double v) {
  total = total - entries[idx];
  entries[idx] = v;
  total = total + v;
  idx = idx + 1;
  if (idx >= depth) { idx = 0; }
  avg = total / (double) depth;
  if (v > peak) {
    peak = v; 
  }
}

/*
 * Track minute volume
 * capture new data, and sum all values from the past minute
 */
void Fifo::fifoPush(double v, uint32_t t) {
  entries[idx] = v;
  ts[idx] = t;
  idx = idx + 1;
  if (idx >= depth) { idx = 0; }
  total = 0;
  int tmpRR = 0;
  for (int i = 0; i < depth; i++) {
    if (ts[i] > t) {  // overflow
      if (UINT32_MAX - ts[i] + t < 60 * 1000) {
        total += entries[i];
        tmpRR++;
      }
    } else {
      if (t - ts[i] < 60 * 1000) {
        total += entries[i];
        tmpRR++;
      }
    }
  }
  minvol = total;
  rrMeas = tmpRR;
}

void Fifo::fifoInit() {
  for (int idx = 0; idx < depth; idx++) {
    entries[idx] = 0;
  }
  total = 0;
}

/*
 * track minute volume
 * create a fake history, which adds up to the desired minute volume
 */
void Fifo::fifoInit(int rr, int tv) {
  depth = rr;
  uint32_t now = millis();
  int period = round( 60 * 1000 / float(rr));  // breath period in ms
  for (int i = 0; i < depth; i++) {
    entries[i] = tv;
    uint32_t dt = period * (i - depth + 1);
    if (dt > now) {  // underflow
      ts[i] = UINT32_MAX - dt + now;
    } else {
      ts[i] = now - dt;
    }
  }
  idx = 0;
  minvol = float(rr * tv);
  rrMeas = rr;
}
