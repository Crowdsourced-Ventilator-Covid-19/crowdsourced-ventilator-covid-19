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
    Fifo(int d);    // d = depth
    void fifoPush(double v, uint32_t t);
    void fifoPush(double v);
    void fifoInit();
    void fifoInit(int rr, int tv);
    double avg;
    double peak;
    double minvol;
    int rrMeas;
  private:
    int depth;
    double entries[40];
    uint32_t ts[40];
    int idx;
    double total;
};

#endif
