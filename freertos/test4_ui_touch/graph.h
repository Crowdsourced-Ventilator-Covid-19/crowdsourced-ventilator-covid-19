#ifndef Graph_h
#define Graph_h

#include <Adafruit_HX8357.h>

class Graph
{
    public:
        Graph();
        Graph(Adafruit_HX8357 &d, double gx, double gy, double w, double h, double xlo, double xhi,
            double xinc, double ylo, double yhi, double yinc, String title, String xlabel, String ylabel, unsigned int gcolor,
            unsigned int acolor, unsigned int pcolor, unsigned int tcolor, unsigned int bcolor, bool xticks);
        void plot(uint32_t t_ms, double y);
        void draw();
    private:
        void _update(double x, double y, bool redraw);
        Adafruit_HX8357 *d;
        double gx;
        double gy;
        double w;
        double h;
        double xlo;
        double xhi;
        double xinc;
        double ylo;
        double yhi;
        double yinc;
        String title;
        String xlabel;
        String ylabel;
        unsigned int gcolor;
        unsigned int acolor;
        unsigned int pcolor;
        unsigned int tcolor;
        unsigned int bcolor;
        double ox;
        double oy;
        double ot;
        bool xticks;
};

#endif