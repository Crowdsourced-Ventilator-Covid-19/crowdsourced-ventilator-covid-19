/*
 * Ventilator touch UI demo
 * Tested using this display: https://www.amazon.com/gp/product/B07NWH47PV/ref=ppx_yo_dt_b_asin_title_o05_s00?ie=UTF8&psc=1
 * Others displays should work as well, if you use the appropriate driver
 * 
 * This code assumes you have a pressure sensor, and a flow sensor.
 * volume measurements are integrated from the flow
 * 
 */

#define ARDUINO 102
#include <MCUFRIEND_kbv.h>  // use the appropriate driver for your touch TFT display
MCUFRIEND_kbv tft;       
#include <TouchScreen.h>
#include <AMS5915.h>
#include <Fonts/FreeSansBold24pt7b.h>
#include <Fonts/FreeSansBold12pt7b.h>
#include <Fonts/FreeSans12pt7b.h>
#include "Wire.h"
#include "Fifo.h"
#include "defines.h"
extern "C" { 
#include "utility/twi.h"  // from Wire library, so we can do bus scanning
}

// -------- input variables -------
int    rr = 15;             // respiratory rate
int    ier = 1;             // I/E ratio  (actually E/I)
int    pmax = 40;           // max pressure (retract piston)
int    peakAlrm = 30;       // indicate pressure alarm
int    mvhiAlrm = 20;       // minute vol hi alarm
int    mvloAlrm = 1;        // minute vol lo alarm
int    dcAlrm = 3;          // disconnect alarm
int    tvSet = 300;         // TV set value
int    trig = 50;           // TV patient trigger threshold

// -------- main variables -------
String screen;
boolean inspPhase = false;  // phase variable
boolean measPend = 0;       // flag to indicate a measurement is needed
boolean power = false;      // start with it turned off

// measured and processed variables
double peep = 0;            // PEEP measurement
double peak = 0;            // Ppeak measurement
double plat = 0;            // Pplat measurement
double tvMeas = 0;          // measured TV

// thresholds
const double peepError = 10;       // TV thresold for measuring steady state PEEP
const double flowDeadZone = 3;    // ignore flow below 3lpm to reduce integration error

// calibration offset vars
double poff = 0;
double foff = 0;
double tvoff = 0;

// temp variables
double tmpP = 0;            // tmp var for pressure
uint32_t tmpP_t = 0;          // tmp var for pressure timestamp
double tmpF = 0;            // tmp var for flow
uint32_t tmpF_t = 0;          // tmp var for flow timestamp
double tmpTv = 0;           // tmp variable for TV
int    omodVal;             // old mod val

// timers
uint32_t breathTimer;       // next timestamp to start breath
uint32_t ierTimer;          // inspiratory phase duration

// touchscreen calibration
const int XP=8,XM=A2,YP=A3,YM=9; //ID=0x9341
const int TS_LEFT=94,TS_RT=956,TS_TOP=912,TS_BOT=118;
TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);
TSPoint tp;
uint16_t tftID;

// these are the only external variables used by the graph function
// it's a flag to draw the coordinate system only on the first call to the Graph() function
// and will mimize flicker
// also create some variables to store the old x and y, if you draw 2 graphs on the same display
// you will need to store ox and oy per each display
int display1 = 2;               // redraw level 2 means draw axes and chart
int display2 = 2;
int display3 = 2;
double ox1 = -999, oy1 = -999;  // Force them to be off screen
double ox2 = -999, oy2 = -999; 
double ox3 = -999, oy3 = -999; 
double last_t = -999;           // track wraparound

// setup the AMS5915 differential pressure sensor
AMS5915 ams0;
AMS5915 ams1;

// setup Fifo
Fifo fifoP(10);
Fifo fifoMv(15);

void setup() {
  Serial.begin(115200);

  // initialize the TFT
  tft.reset();
  tftID = tft.readID();
  tft.begin(tftID);
  tft.setRotation(1);
  
  Wire.begin();
  // initialize sensors and check health
  diag();

  fifoMv.fifoInit(rr, tvSet);

  // interrupt driven pressure and flow sample rate to workaround slow screen refresh
  cli();//stop interrupts
  // TIMER 1 for interrupt frequency 100 Hz:
  cli(); // stop interrupts
  TCCR1A = 0; // set entire TCCR1A register to 0
  TCCR1B = 0; // same for TCCR1B
  TCNT1  = 0; // initialize counter value to 0
  // set compare match register for 100 Hz increments
  OCR1A = 19999; // = 16000000 / (8 * 100) - 1 (must be <65536)
  // turn on CTC mode
  TCCR1B |= (1 << WGM12);
  // Set CS12, CS11 and CS10 bits for 8 prescaler
  TCCR1B |= (0 << CS12) | (1 << CS11) | (0 << CS10);
  // enable timer compare interrupt
  TIMSK1 |= (1 << OCIE1A);
  sei(); // allow interrupts

  // Reset Breath and I/E timers
  resetTimers();
  
  // configure the piston control pin
  pinMode(PISTON, OUTPUT);
  digitalWrite(PISTON, LOW);
  pinMode(SOLENOID, OUTPUT);
  digitalWrite(SOLENOID, LOW);
  pinMode(ALARM, OUTPUT);
  digitalWrite(ALARM, LOW);

  drawSetScreen();
}

void loop() {
  // handler for user interaction
  checkTouch();

  measLoop();
  if (screen == "main") { updateGraphs(); }
  delay(50);
}

// interrupt handler sets flag inidicating that a new measurement is required
ISR(TIMER1_COMPA_vect){//timer1 interrupt 100Hz
  measPend = 1;
}

// measLoop handles sampling, error checking and state tracking.
// call this function often, as it will only run if the interrupt driven measPend flag is true
void measLoop() {
  if (measPend) {
    // read patient pressure & timestamp
    tmpP = readPressure(ams1, 1);
    fifoP.fifoPush(tmpP);
    tmpP_t = millis();
    //Serial.println("phase: " + String(inspPhase) + " conv: " + String(fifoDp.converging));

    // check if we've hit the hard pressure limit
    if (tmpP >= pmax) {
      //Serial.println("P = " + String(tmpP));
      digitalWrite(PISTON, LOW); // release BVM bag
    }

    //Serial.println("Phase: " + String(inspPhase) + " P: " + String(tmpP) + " Peak: " + String(tmpPeak));
    // TODO overpressure alarm check here
  
    
    // read new flow measure, and ignore nan return values
    float f;
    f = readFlow(ams0, 0);
    if (isnan(f) == false) {
  
      // save old flow values
      double ot = tmpF_t;
      double of = tmpF;
      
      // write new flow values
      tmpF_t = millis();
      tmpF = f;


      // handle time wraparound case
      uint32_t dt;
      if (tmpF_t < ot) {
        dt = UINT32_MAX - ot + tmpF_t;
      } else {
        dt = tmpF_t - ot;
      }
      // trapezoidal intgegration to track volume (lpm*s -> cc)
      // Serial.println("dt: " + String(dt) + " of: " + String(of) + " tmpF: " + String(tmpF) + " tmpTv: " + String(tmpTv));
      tmpTv += float(dt) * (of + tmpF) / 2.0 / 60.0;
    }

    // check if we've hit desired TV
    // Serial.println("TV = " + String(tmpTv) + "/" + String(tvSet) + " tvoff = " + String(tvoff));
    if (tmpTv >= tvSet + tvoff) {
      digitalWrite(PISTON, LOW); // release BVM bag
    }
  
    // ------------ inspiratory phase --------------
    if (inspPhase) {
      // TODO disconnect alarm here
  
      // check if inspiratory phase is over
      if (millis() > ierTimer) {
        digitalWrite(PISTON, LOW);   // release BVM bag
        if (power) {
          digitalWrite(SOLENOID, HIGH);  // open expiratory path
        }
        inspPhase = false;
        // Serial.println("exp");
        peak = fifoP.peak;
        //Serial.println(peak);
        plat = fifoP.avg;
        tvMeas = tmpTv;
        fifoMv.fifoPush(tvMeas, millis());
        // Serial.println(fifoMv.minvol);

        if (tvMeas > 99) {
          tvoff += tvSet - tmpTv; // simple proportional control
          tvoff = min(tvoff, 0.1 * float(tvSet));
          tvoff = max(tvoff, -0.1 * float(tvSet));
        }
        Serial.println(tvoff);
        tmpTv = 0; // no expiratory flow meter, assume TV goes to zero
        updateMeasures();
      }
      
    // ------------ expiratory phase --------------
    } else {
      
      // TODO plat alarm here
  
      // if the pressure is stable, then capture as PEEP
      if (tmpTv < peepError) { 
        peep = tmpP; 
      }

      // Serial.println("peep: " + String(peep) + " tmpTv: " + String(tmpTv));
      // TODO PEEP alarm here
      
      // Check if time to start a new breath
      if (millis() > breathTimer || (trig > 0 && tmpTv > trig)) {
        inspPhase = true;
        // Serial.println("insp");
        fifoP.peak = 0;              // reset PIP tracking
        fifoP.fifoInit();
        if (power) {
          digitalWrite(PISTON, HIGH);  // compress BVM bag
        }
        digitalWrite(SOLENOID, LOW); // close expiratory path
        resetTimers();
      }
    } 
    
    // reset measure pending flag
    measPend = 0;
  }
}

// every time we trigger a new breath, reset breath and I/E timers
void resetTimers() {
  // start the breath timer
  breathTimer = round(60.0 / rr * 1000.0) + millis();
  ierTimer = round((60.0 / rr * 1000.0) * (1.0 - (float(ier) / (float(ier)+1.0)))) + millis();
}
