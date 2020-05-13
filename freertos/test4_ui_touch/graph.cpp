#include "graph.h"
#include <Adafruit_HX8357.h>
#ifndef FONTS_h
#define FONTS_h
#include <Fonts/FreeSansBold24pt7b.h>
#include <Fonts/FreeSansBold12pt7b.h>
#include <Fonts/FreeSans12pt7b.h>
#endif

Graph::Graph() {};

Graph::Graph(Adafruit_HX8357 &d, double gx, double gy, double w, double h, double xlo, double xhi,
  double xinc, double ylo, double yhi, double yinc, String title, String xlabel, String ylabel, unsigned int gcolor,
  unsigned int acolor, unsigned int pcolor, unsigned int tcolor, unsigned int bcolor) {
    this->d = &d;
    this->gx = gx;
    this->gy = gy;
    this->w = w;
    this->h = h;
    this->xlo = xlo;
    this->xhi = xhi;
    this->xinc = xinc;
    this->ylo = ylo;
    this->yhi = yhi;
    this->yinc = yinc;
    this->title = title;
    this->xlabel = xlabel;
    this->ylabel = ylabel;
    this->gcolor = gcolor;
    this->acolor = acolor;
    this->pcolor = pcolor;
    this->tcolor = tcolor;
    this->bcolor = bcolor;
    ox = -999;
    oy = -999;
    ot = 999;
  }


// draw the graph axes
void Graph::draw() {
  _update(-1,0,true);
  ox = -999;
  oy = -999;
}

// plot data point on graph
// if x-axis overflow, clear the graph and start from time 0
void Graph::plot(uint32_t t_ms, double y) {
  double x = float(t_ms % 15000) / 1000.0;
  _update(x, y, (x < ot));
  ot = x;
}

void Graph::_update(double x, double y, bool redraw) {
  double ydiv, xdiv;
  double i;
  double temp;
  int rot, newrot;

  if (redraw) {
    d->fillRect(gx, gy - h, w, h+5, bcolor);

    ox = (x - xlo) * ( w) / (xhi - xlo) + gx;
    oy = (y - ylo) * (gy - h - gy) / (yhi - ylo) + gy;
    // draw y scale
    d->setTextSize(1);
    d->setTextColor(tcolor, bcolor);
    d->setFont();
    for ( i = ylo; i <= yhi; i += yinc) {
      // compute the transform
      temp =  (i - ylo) * (gy - h - gy) / (yhi - ylo) + gy;

      if (i == 0) {
        d->drawLine(gx, temp, gx + w, temp, acolor);
      }
      else {
         d->drawLine(gx, temp, gx + w, temp, gcolor);
      }
      d->setCursor(gx - 30, temp);
      d->println((int)i);
    }
    // draw x scale
    d->setTextSize(1);
    d->setTextColor(tcolor, bcolor);
    d->setFont();
    for (i = xlo; i <= xhi; i += xinc) {

      // compute the transform
      temp =  (i - xlo) * ( w) / (xhi - xlo) + gx;
      if (i == 0) {
        d->drawLine(temp, gy, temp, gy - h, acolor);
      }
      else {
        d->drawLine(temp, gy, temp, gy - h, gcolor);
      }
      d->setCursor(temp, gy + 10);
      d->println((int)i);
    }

    //now draw the labels
    d->setTextSize(1);
    d->setFont(&FreeSans12pt7b);
    d->setTextColor(tcolor, bcolor);
    d->setCursor(gx , gy - h + 20);
    d->println(title);

    if (redraw) {
      d->setFont();
      d->setTextSize(1);
      d->setTextColor(acolor, bcolor);
      d->setCursor(gx , gy + 20);
      d->println(xlabel);
      d->setFont();
      d->setTextSize(1);
      d->setTextColor(acolor, bcolor);
      d->setCursor(gx - 10, gy - h - 10);
      d->println(ylabel);
    }
  }

  if (y <= yhi && y >= ylo) { // dont draw off chart
    //graph drawn now plot the data
    // the entire plotting code are these few lines...
    // recall that ox and oy are initialized as static above
    x =  (x - xlo) * ( w) / (xhi - xlo) + gx;
    y =  (y - ylo) * (gy - h - gy) / (yhi - ylo) + gy;
  
    if (x - ox < 30 && x > ox) {
      d->drawLine(ox, oy, x, y, pcolor);
      d->drawLine(ox, oy + 1, x, y + 1, pcolor);
      d->drawLine(ox, oy - 1, x, y - 1, pcolor);
    }
  }
  ox = x;
  oy = y;

}
