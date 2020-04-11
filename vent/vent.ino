/*

  This program provides cartesian type graph function

  Revisions
  rev     date        author      description
  1       12-24-2015  kasprzak    initial creation

  Updated by Bodmer to be an example for the library here:
  https://github.com/Bodmer/TFT_HX8357
  
*/
// use the standard I2C port for the patient pressure sensor
#include <Wire.h>

// use a software I2C port for the atmos pressure reference
#define SCL_PIN 2
#define SCL_PORT PORTD
#define SDA_PIN 3
#define SDA_PORT PORTD
#include "SoftWire.h"
SoftWire sWire = SoftWire();


#define BLACK     0x0000
#define YELLOW    0xFFE0

// Adafruit pressure sensor library
#include "Adafruit_MPRLS.h"
// You dont *need* a reset and EOC pin for most uses, so we set to -1 and don't connect
#define RESET_PIN  -1  // set to any GPIO pin # to hard-reset on begin()
#define EOC_PIN    -1  // set to any GPIO pin to read end-of-conversion by pin
#define RELAY       8
#define EXP_RELAY   9  // expiratory solenoid
Adafruit_MPRLS mpr = Adafruit_MPRLS(RESET_PIN, EOC_PIN);
Adafruit_MPRLS smpr = Adafruit_MPRLS(RESET_PIN, EOC_PIN);

// TFT library
#include <TFT_HX8357.h> // Hardware-specific library
TFT_HX8357 tft = TFT_HX8357();       // Invoke custom library


// -------- input variables -------
// these will eventually be set by the touch UI
int    rr = 15;             // respiratory rate
double ier = 2;             // I/E ratio  (actually E/I)
int    peakAlrm = 35;
int    platAlrm = 30;
int    peepAlrm = 10;
int    peakLmt = 40;
int    trigger = -3;

// -------- main variables -------
double peep = 0;            // PEEP measurement
double peak = 0;            // Ppeak
double plat = 0;            // Pplat
double tmpPeak = 0;         // tmp variable for peak measurements
double tmpPlat = 0;         // tmp variable for plat measurements
double peepError = 0.5;     // thresold for measuring steady state PEEP
boolean inspPhase = false;  // phase variable
uint32_t breathTimer;       // next timestamp to start breath
uint32_t ierTimer;          // inspiratory phase duration 
uint32_t pAtmosTimer = 0;   // timer to take a new atmos pressure read
double p_atmos_hPa = 0;     // atmos pressure in hectoPascals
uint32_t pAtmosDelay = 1000 * 60 ;  // take new atmos reading every minute


// these are the only external variables used by the graph function
// it's a flag to draw the coordinate system only on the first call to the Graph() function
// and will mimize flicker
// also create some variables to store the old x and y, if you draw 2 graphs on the same display
// you will need to store ox and oy per each display
boolean display1 = true;
boolean update1 = true;
double ox = -999, oy = -999; // Force them to be off screen
double lastx = -999;

// ------- 10 entry FIFO for sample averaging -------
// Define the number of samples to keep track of. The higher the number, the
// more the readings will be smoothed, but the slower the output will respond to
// the input. Using a constant rather than a normal variable lets us use this
// value to determine the size of the readings array.
const int numReadings = 10;
double readings[numReadings];      // the readings from the analog input
int readIndex = 0;              // the index of the current reading
double total = 0;                  // the running total
double average = 0;                // the average

// --------- graph dimensions ---------
int ymax = 45;
int ymin = -5;
int ygrid = 5;
int xmax = 10;
int xmin = 0;
int xgrid = 1;


void setup() {
  double x, y;
  double lastx;  // variable to track xaxis wraparound
  
  // initialize the pressure sensors
  mpr.begin();
  smpr.begin();

  // prepare TFT screen
  tft.begin();
  tft.fillScreen(BLACK);
  tft.setRotation(1);
  // draw the initial graph axes
  Graph(tft, x, y, 1, 60, 290, 390, 180, xmin, xmax, xgrid, ymin, ymax, ygrid, "", "", "", display1, YELLOW);

  // Reset Breath and I/E timers
  resetTimers();
  // initialize all the FIFO readings to 0:
  fifoInit();
  
  // configure the RELAY control pin
  pinMode(RELAY, OUTPUT);
  digitalWrite(RELAY, HIGH);

  pinMode (EXP_RELAY, OUTPUT);
  digitalWrite(EXP_RELAY, HIGH);
}


void loop(void) {
  boolean redraw = true;
  // read guage pressure
  double p_cmh2o = readPressure();

  // TODO overpressure alarm check here
  if (p_cmh2o > peakLmt) {
    digitalWrite(RELAY, HIGH); // hardlimit, release bag
    // inspPhase = false
  }

  // ------------ inspiratory phase --------------
  if (inspPhase) {
    // TODO disconnect alarm here
    
    // track the max pressure during inspiratory phase
    if (p_cmh2o > tmpPeak) { tmpPeak = p_cmh2o; }

    // check if inspiratory phase is over
    if (millis() > ierTimer) {
      digitalWrite(RELAY, HIGH); // release BVM bag
      digitalWrite(EXP_RELAY, LOW);
      inspPhase = false;
    }
    
  // ------------ expiratory phase --------------
  } else {
    
    // capture values when detecting a phase change to expiratory
    if (tmpPeak > 0) {
      peak = tmpPeak;
      plat = average;
    }
    tmpPeak = 0;
    // TODO plat alarm here

    // if the pressure is stable, then capture as PEEP
    if (abs(p_cmh2o - average) < peepError) { 
      peep = p_cmh2o; 
    }
    // TODO PEEP alarm here

    // look for patient assist
    //if (p_cmh2o + trigger < peep) {
    //  inspPhase = true;
    //  digitalWrite(RELAY, LOW); // patient assist
    //  digitalWrite(EXP_RELAY, HIGH);
    //  resetTimers();
    //}
    
    // Check if time to start a new breath
    if (millis() > breathTimer) { 
      inspPhase = true;
      digitalWrite(RELAY, LOW); // compress BVM bag
      digitalWrite(EXP_RELAY, HIGH);
      resetTimers();

    }
  } 

  // ------- FIFO running average code ---------
  average = fifoAvg(p_cmh2o);

  // ------- Text display --------
  tft.setCursor(0,0,4);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.print("Ppeak: "); tft.print(peak); tft.print("    ");
  tft.print("Pplat: "); tft.print(plat); tft.print("    ");
  tft.print("PEEP: "); tft.print(peep); tft.println("    ");
  tft.print("RR: "); tft.print(rr); tft.print("    ");
  tft.print("I/E: "); tft.print(ier); tft.print("    ");
  if (inspPhase) {
    tft.println("Inspiratory     ");
  } else {
    tft.println("Expiratory      ");
  }

  // -------- Draw Chart ---------
  double x = (millis() % 10000) / 1000.0;
  double y = p_cmh2o;

  // detect screen wrap, clear the old waves
  if (lastx > x) {
    ox = x + 60;
    tft.fillRect(1, 100, 480, 200, BLACK);
    Graph(tft, x, y, 1, 60, 290, 390, 180, xmin, xmax, xgrid, ymin, ymax, ygrid, "Pressure", "Time(s)", "cmH2o", redraw, YELLOW);
  }
  Trace(tft, x, y, 1, 60, 290, 390, 180, xmin, xmax, xgrid, ymin, ymax, ygrid, "Pressure", "Time(s)", "cmH2o", update1, YELLOW);
  lastx = x;
  delay(50);
}

// every time we trigger a new breath, reset breath and I/E timers
void resetTimers() {
  // start the breath timer
  breathTimer = round(60.0 / rr * 1000.0) + millis();
  ierTimer = round((60.0 / rr * 1000.0) * (1 - (ier / (ier+1)))) + millis();
}

// read patient and reference pressure sensors to calculate guage pressure
double readPressure() {
  if (millis() > pAtmosTimer || p_atmos_hPa == 0) {
    p_atmos_hPa = smpr.readPressure();
    pAtmosTimer = millis() + pAtmosDelay;
    delay(50);
  }
  // read pressure sensor in hectoPa
  double p_hPa = mpr.readPressure();
  
  // convert to relative pressure in cmh2o
  return (p_hPa - p_atmos_hPa) * 1.01974428892;
}

// average the last numReadings samples to dampen noise, and detect steady-state
double fifoAvg(int p) {
  total = total - readings[readIndex];
  // read from the sensor:
  readings[readIndex] = p;
  // add the reading to the total:
  total = total + readings[readIndex];
  // advance to the next position in the array:
  readIndex = readIndex + 1;
  
  // if we're at the end of the array...
  if (readIndex >= numReadings) {
    // ...wrap around to the beginning:
    readIndex = 0;
  }
  
  // calculate the average:
  double avg = total / numReadings;
  return avg;
}

// initialize the fifo values to zero
void fifoInit() {
  for (int thisReading = 0; thisReading < numReadings; thisReading++) {
    readings[thisReading] = 0;
  }
}
