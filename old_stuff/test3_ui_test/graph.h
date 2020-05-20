#ifndef Graph_h
#define Graph_h

#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_HX8357.h>

void Graph(Adafruit_HX8357 &d, double x, double y, double gx, double gy, double w, double h, double xlo, double xhi,
double xinc, double ylo, double yhi, double yinc, String title, String xlabel, String ylabel, unsigned int gcolor,
unsigned int acolor, unsigned int pcolor, unsigned int tcolor, unsigned int bcolor, int redraw, double *ox, double *oy);

#endif