/* 
 *  Fifo.h - Library for a custom ring buffer with a user specified depth
 *  It tracks the average of all entries
 *  It can also track the derivative of incoming measurements
 *  and report if that dervative is converging to zero or diverging
 */

#ifndef Fsens_h
#define Fsens_h

class Fsens
{
  public:
    Fsens(int tca, double d);
    void read();
    double f;
    uint32_t t;
    double foff;
    double tv;
  private:
    int tca;
    const float AMS5915_0100_D_MIN = 0.0;       // minimum pressure, millibar
    const float AMS5915_0100_D_MAX = 100.0;     // maximum pressure, millibar
    void tcaselect(int i);
    double deadzone;
    double flow;
    uint32_t ot;
    double of;
};

#endif
