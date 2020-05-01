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
    void fifoPush(double v);
    void fifoInit();
    double avg;
    double peak;
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
