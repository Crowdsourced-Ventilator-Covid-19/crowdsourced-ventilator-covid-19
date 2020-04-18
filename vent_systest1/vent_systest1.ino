/*
 * Ventilator touch UI demo
 * Tested using this display: https://www.amazon.com/gp/product/B07NWH47PV/ref=ppx_yo_dt_b_asin_title_o05_s00?ie=UTF8&psc=1
 * Others displays should work as well, if you use the appropriate driver
 * 
 * This code assumes you have a pressure sensor, and a flow sensor.
 * volume measurements are integrated from the flow
 * 
 */


#include <MCUFRIEND_kbv.h>  // use the appropriate driver for your touch TFT display
MCUFRIEND_kbv tft;       
#include <TouchScreen.h>
#include <AMS5915.h>
#include <Fonts/FreeSansBold24pt7b.h>
#include <Fonts/FreeSansBold12pt7b.h>
#include <Fonts/FreeSans12pt7b.h>
#include "Wire.h"
extern "C" { 
#include "utility/twi.h"  // from Wire library, so we can do bus scanning
}


#define LTBLUE    0xB6DF
#define LTTEAL    0xBF5F
#define LTGREEN   0xBFF7
#define LTCYAN    0xC7FF
#define LTRED     0xFD34
#define LTMAGENTA 0xFD5F
#define LTYELLOW  0xFFF8
#define LTORANGE  0xFE73
#define LTPINK    0xFDDF
#define LTPURPLE  0xCCFF
#define LTGREY    0xE71C

#define BLUE      0x001F
#define TEAL      0x0438
#define GREEN     0x07E0
#define CYAN      0x07FF
#define RED       0xF800
#define MAGENTA   0xF81F
#define YELLOW    0xFFE0
#define ORANGE    0xFC00
#define PURPLE    0x8010
#define GREY      0xC618
#define WHITE     0xFFFF
#define BLACK     0x0000

#define DKBLUE    0x000D
#define DKTEAL    0x020C
#define DKGREEN   0x03E0
#define DKCYAN    0x03EF
#define DKRED     0x6000
#define DKMAGENTA 0x8008
#define DKYELLOW  0x8400
#define DKORANGE  0x8200
#define DKPINK    0x9009
#define DKPURPLE  0x4010
#define DKGREY    0x4A49

#define Orientation 1



// -------- input variables -------
int    rr = 15;             // respiratory rate
int    ier = 1;             // I/E ratio  (actually E/I)
int    pmax = 40;           // max pressure (retract piston)
int    peakAlrm = 30;       // indicate pressure alarm
int    platAlrm = 20;       // indicate plateau alarm
int    peepAlrm = 10;       // indicate PEEP alarm
int    tvSet = 300;         // TV set value
int    negTrig = -3;        // negative trigger assist threshold

// -------- main variables -------
String screen;
boolean inspPhase = false;  // phase variable

// measured and processed variables
double peep = 0;            // PEEP measurement
double peak = 0;            // Ppeak measurement
double plat = 0;            // Pplat measurement
double tvMeas = 0;          // measured TV
double p_atmos_hPa = 0;     // atmos pressure in hectoPascals

// calibration vars
double poff = 0;
double foff = 0;
double tvoff = 0;

// temp variables
double tmpP = 0;            // tmp var for pressure
double tmpP_t = 0;          // tmp var for pressure timestamp
double tmpF = 0;            // tmp var for flow
double tmpF_t = 0;          // tmp var for flow timestamp
double tmpTv = 0;           // tmp variable for TV
double tmpPeak = 0;         // tmp variable for peak measurements
double tmpPlat = 0;         // tmp variable for plat measurements
String modVar;              // variable being modified
int    modVal;              // placeholder for new variable
int    omodVal;             // old mod val
boolean measPend = 0;       // flag to indicate a measurement is needed
// double peakFlow = 0;     // used for flow calibration using a PEF meter


// thresholds
double peepError = 2.0;     // thresold for measuring steady state PEEP
double flowDeadZone = 3;    // ignore flow below 3lpm to reduce integration error

// timers
uint32_t breathTimer;       // next timestamp to start breath
uint32_t ierTimer;          // inspiratory phase duration
uint32_t pAtmosTimer = 0;   // timer to take a new atmos pressure read
uint32_t pAtmosDelay = 1000 * 60UL;  // take new atmos reading every minute

// touchscreen calibration
const int XP=8,XM=A2,YP=A3,YM=9; //ID=0x9341
const int TS_LEFT=94,TS_RT=956,TS_TOP=912,TS_BOT=118;
TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);
TSPoint tp;
#define MINPRESSURE 200
#define MAXPRESSURE 1000
uint16_t ID;

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

// Adafruit pressure sensor library
#include "Adafruit_MPRLS.h"
// You dont *need* a reset and EOC pin for most uses, so we set to -1 and don't connect
#define RESET_PIN  -1  // set to any GPIO pin # to hard-reset on begin()
#define EOC_PIN    -1  // set to any GPIO pin to read end-of-conversion by pin
Adafruit_MPRLS mpr = Adafruit_MPRLS(RESET_PIN, EOC_PIN);

// outputs
#define PISTON     23  // 5-way, 2pos pneumatic solenoid controller
#define SOLENOID   25  // expiratory path solenoid
#define ALARM      27  // alarm

// setup the AMS5915 differential pressure sensor
AMS5915 ams;

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

void setup() {
  Serial.begin(115200);

  // initialize the TFT
  tft.reset();
  ID = tft.readID();
  tft.begin(ID);
  tft.setRotation(1);
  
  Wire.begin();
  // initialize sensors and check health
  diag();

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
  // initialize all the FIFO readings to 0:
  fifoInit();

  // configure the piston control pin
  pinMode(PISTON, OUTPUT);
  digitalWrite(PISTON, HIGH);
  pinMode(SOLENOID, OUTPUT);
  digitalWrite(SOLENOID, LOW);
  pinMode(ALARM, OUTPUT);
  digitalWrite(ALARM, LOW);

  drawMainScreen();
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
    tmpP = readPressure();
    tmpP_t = (millis() % 15000) / 1000.0;

    // check if we've hit the hard pressure limit
    if (tmpP >= pmax) {
      digitalWrite(PISTON, HIGH); // release BVM bag
    }

    // TODO overpressure alarm check here
  
    
    // read new flow measure, and ignore nan return values
    float f;
    f = readFlow();
    if (isnan(f) == false) {

      // if (f > peakFlow) { peakFlow = f; }
      
      // save old flow values
      double ot = tmpF_t;
      double of = tmpF;
      
      // write new flow values
      tmpF_t = (millis() % 15000) / 1000.0;
      tmpF = f;

      // handle time wraparound case
      double nt = tmpF_t;
      if (ot > nt) { nt += 15.0; }

      // trapezoidal intgegration to track volume (lpm*s -> cc)
      tmpTv += 1000/60 * (nt - ot) * (of + tmpF) /2;
    }

    // check if we've hit desired TV
    if (tmpTv >= tvSet + tvoff) {
      digitalWrite(PISTON, HIGH); // release BVM bag
    }
  
    // ------------ inspiratory phase --------------
    if (inspPhase) {
      // TODO disconnect alarm here
      
      // track the max pressure during inspiratory phase
      if (tmpP > tmpPeak) { tmpPeak = tmpP; }
  
      // check if inspiratory phase is over
      if (millis() > ierTimer) {
        digitalWrite(PISTON, HIGH);   // release BVM bag
        digitalWrite(SOLENOID, LOW);  // open expiratory path
        inspPhase = false;
        peak = tmpPeak;
        tmpPeak = 0;
        plat = average;
        tvMeas = tmpTv;
        tmpTv = 0; // no expiratory flow meter, assume TV goes to zero
        tvoff += tvSet - tmpTv; // simple proportional control
        //Serial.println(tvoff);
        // Serial.println(peakFlow);
        // peakFlow = 0;
        updateMeasures();
      }
      
    // ------------ expiratory phase --------------
    } else {
      
      // TODO plat alarm here
  
      // if the pressure is stable, then capture as PEEP
      if (abs(tmpP - average) < peepError) { 
        peep = tmpP; 
      }
      // TODO PEEP alarm here
      
      // Check if time to start a new breath
      if (millis() > breathTimer) { 
        inspPhase = true;
        digitalWrite(PISTON, LOW); // compress BVM bag
        digitalWrite(SOLENOID, HIGH); // close expiratory path
        resetTimers();
  
      }
    } 

    // ------- FIFO running average code ---------
    average = fifoAvg(tmpP);  
  
    
    // reset measure pending flag
    measPend = 0;
  }
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

// every time we trigger a new breath, reset breath and I/E timers
void resetTimers() {
  // start the breath timer
  breathTimer = round(60.0 / rr * 1000.0) + millis();
  ierTimer = round((60.0 / rr * 1000.0) * (1.0 - (float(ier) / (float(ier)+1.0)))) + millis();
}
