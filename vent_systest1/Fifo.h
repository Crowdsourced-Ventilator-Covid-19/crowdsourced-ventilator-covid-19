/* 
 *  Fifo.h - Library for a custom ring buffer with a user specified depth
 *  It tracks the average of all entries
 *  It can also track the derivative of incoming measurements
 *  and report if that dervative is converging to zero or diverging
 */

#ifndef Fifo_h
#define Fifo_h

class Fifo
{
  public:
    Fifo(int d, double n);    // d = depth,  n = sensor noise to ignore when checking convergence
    void fifoPush(double v);
    void fifoPushDeriv(double v, uint32_t t);
    double avg;
    double peak;
    boolean converging;
  private:
    int depth;
    double entries[10];
    uint32_t last_t;
    double last_v;
    int idx;
    double total;
    double noise;
};

#endif
