/*
  function to draw a cartesian coordinate system and plot whatever data you want
  just pass x and y and the graph will be drawn
  huge arguement list
  &d name of your display object
  x = x data point
  y = y datapont
  gx = x graph location (lower left)
  gy = y graph location (lower left)
  w = width of graph
  h = height of graph
  xlo = lower bound of x axis
  xhi = upper bound of x asis
  xinc = division of x axis (distance not count)
  ylo = lower bound of y axis
  yhi = upper bound of y asis
  yinc = division of y axis (distance not count)
  title = title of graph
  xlabel = x asis label
  ylabel = y asis label
  gcolor = graph line colors
  acolor = axi ine colors
  pcolor = color of your plotted data
  tcolor = text color
  bcolor = background color
  &redraw = 2 = full redraw, 1 = clear chart area, 0 = no redraw
  yticks = boolean draw xticks
*/


void Graph(MCUFRIEND_kbv &d, uint32_t x_ms, double y, double gx, double gy, double w, double h, double xlo, double xhi,
double xinc, double ylo, double yhi, double yinc, String title, String xlabel, String ylabel, unsigned int gcolor,
unsigned int acolor, unsigned int pcolor, unsigned int tcolor, unsigned int bcolor, int &redraw, double *ox, double *oy, boolean xticks) {
  double ydiv, xdiv;
  double x;
  // initialize old x and old y in order to draw the first point of the graph
  // but save the transformed value
  // note my transform funcition is the same as the map function, except the map uses long and we need doubles
  //static double ox = (x - xlo) * ( w) / (xhi - xlo) + gx;
  //static double oy = (y - ylo) * (gy - h - gy) / (yhi - ylo) + gy;
  double i;
  double temp;
  int rot, newrot;
  x = float(x_ms % 15000) / 1000.0;  // convert millis time to seconds mod 15

  if (redraw > 0) {
    d.fillRect(gx, gy - h, w, h+5, BLACK);

    *ox = (x - xlo) * ( w) / (xhi - xlo) + gx;
    *oy = (y - ylo) * (gy - h - gy) / (yhi - ylo) + gy;
    // draw y scale
    d.setTextSize(1);
    d.setTextColor(tcolor, bcolor);
    d.setFont();
    for ( i = ylo; i <= yhi; i += yinc) {
      // compute the transform
      temp =  (i - ylo) * (gy - h - gy) / (yhi - ylo) + gy;

      if (i == 0) {
        d.drawLine(gx, temp, gx + w, temp, acolor);
      }
      else {
         d.drawLine(gx, temp, gx + w, temp, gcolor);
      }
      if (redraw > 1) {
        d.setCursor(gx - 30, temp);
        d.println((int)i);
      }
    }
    measLoop();
    // draw x scale
    d.setTextSize(1);
    d.setTextColor(tcolor, bcolor);
    d.setFont();
    for (i = xlo; i <= xhi; i += xinc) {

      // compute the transform

      temp =  (i - xlo) * ( w) / (xhi - xlo) + gx;
      if (i == 0) {
        d.drawLine(temp, gy, temp, gy - h, acolor);
      }
      else {
        d.drawLine(temp, gy, temp, gy - h, gcolor);
      }

      if (xticks && redraw > 1) {
        d.setCursor(temp, gy + 10);
        d.println((int)i);
      }
    }
    measLoop();

    //now draw the labels
    d.setTextSize(1);
    d.setFont(&FreeSans12pt7b);
    d.setTextColor(tcolor, bcolor);
    d.setCursor(gx , gy - h + 20);
    d.println(title);
    measLoop();

    if (redraw > 1) {
      d.setFont();
      d.setTextSize(1);
      d.setTextColor(acolor, bcolor);
      d.setCursor(gx , gy + 20);
      d.println(xlabel);
      measLoop();
      d.setFont();
      d.setTextSize(1);
      d.setTextColor(acolor, bcolor);
      d.setCursor(gx - 10, gy - h - 10);
      d.println(ylabel);
      measLoop();
    }
    redraw = 0;
  }

  if (y <= yhi && y >= ylo) { // dont draw off chart
    //graph drawn now plot the data
    // the entire plotting code are these few lines...
    // recall that ox and oy are initialized as static above
    x =  (x - xlo) * ( w) / (xhi - xlo) + gx;
    y =  (y - ylo) * (gy - h - gy) / (yhi - ylo) + gy;
  
    if (x - *ox < 30 && x > *ox) {
      d.drawLine(*ox, *oy, x, y, pcolor);
      d.drawLine(*ox, *oy + 1, x, y + 1, pcolor);
      d.drawLine(*ox, *oy - 1, x, y - 1, pcolor);
    }
  }
  *ox = x;
  *oy = y;

}


void clearScreen() {
  tft.fillScreen(BLACK);
  //for (int i = 0; i < 10; i++) {
  //  tft.fillRect(0, i*32, 480, i*32+31, BLACK);
  measLoop();
  //}
}
