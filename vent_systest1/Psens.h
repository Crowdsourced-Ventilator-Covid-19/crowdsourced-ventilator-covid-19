/* 
 *  Fifo.h - Library for a custom ring buffer with a user specified depth
 *  It tracks the average of all entries
 *  It can also track the derivative of incoming measurements
 *  and report if that dervative is converging to zero or diverging
 */

#ifndef Psens_h
#define Psens_h

class Psens
{
  public:
    Psens(int tca);
    void read();
    double p;
    uint32_t t;
    double poff;
  private:
    int tca;
    const float AMS5915_0100_D_MIN = 0.0;       // minimum pressure, millibar
    const float AMS5915_0100_D_MAX = 100.0;     // maximum pressure, millibar
    void tcaselect(int i);
};

#endif
