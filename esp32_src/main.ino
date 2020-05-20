#define SIM_LUNG   // use this to run simulated sensors

#define ARDUINO 10812
#include <Arduino.h>
#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_HX8357.h>
#include <TouchScreen.h>
#include <SPI.h>
#include "Wire.h"

// defines, structs and enums
#include "config.h"
#include "types.h"

// screens
#include "set_screen.h"
#include "main_screen.h"
#include "mod_screen.h"
#include "alarm_screen.h"

// sensor classes
#ifdef SIM_LUNG
#include "sim_lung.h"
#include "sim_psens.h"
#include "sim_fsens.h"
#else
#include "psens.h"
#include "fsens.h"
#endif

// tft screen
Adafruit_HX8357 tft = Adafruit_HX8357(TFT_CS, TFT_DC, TFT_RST);
// For better pressure precision, we need to know the resistance
// between X+ and X- Use any multimeter to read it
// For the one we're using, its 300 ohms across the X plate
TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);

#ifdef SIM_LUNG
// these are simulated sensors for testing without hardware
QueueHandle_t lungQ = xQueueCreate(1, sizeof(Lung_t));  // sim lung
SimPsens psens = SimPsens(lungQ, 1);
SimFsens iFsens = SimFsens(lungQ, 0);
SimFsens eFsens = SimFsens(lungQ, 2);
#else 
// these are the real sensors
Psens psens = Psens(1);   // pressure sensor
Fsens iFsens = Fsens(0);  // inspiratory flow sensor
Fsens eFsens = Fsens(2);  // expiratory flow sensor
#endif

// queuehandles
QueueHandle_t sampleQ = NULL;   // hold sensor readings in 1000 deep queue
QueueHandle_t settingQ = NULL;  // configurable settings, 1 deep queue
QueueHandle_t stateQ = NULL;    // state, 1 deep queue
QueueHandle_t alarmQ = NULL;    // alarm conditions, 1 deep queue

// set this 60s into the future when alarm mute button is pressed
unsigned long alarmMuteTimer = 0;

void setup(void) {
    Wire.begin();
    Serial.begin(115200);

    tft.begin();
    tft.setRotation(3);
    tft.fillScreen(BLACK);

    // calibrate the sensors
    psens.calibrate(tft);
    iFsens.calibrate(tft);
    eFsens.calibrate(tft);
    delay(10000);

    // initialize the queues
    initQ();

    pinMode(PISTON, OUTPUT);
    digitalWrite(PISTON, LOW);
    pinMode(EXPVALVE, OUTPUT);
    digitalWrite(EXPVALVE, LOW);
    pinMode(ALARM, OUTPUT);
    digitalWrite(ALARM, LOW);


    xTaskCreatePinnedToCore(
                    readSensor,          /* Task function. */
                    "ReadSensor",        /* String with name of task. */
                    10000,            /* Stack size in bytes. */
                    NULL,             /* Parameter passed as input of the task */
                    1,                /* Priority of the task. */
                    NULL,             /* Task handle. */
                    0);               /* pin to core 0 */

    #ifdef SIM_LUNG
    xTaskCreatePinnedToCore(
                    simLung,          /* Task function. */
                    "SimLung",        /* String with name of task. */
                    10000,            /* Stack size in bytes. */
                    NULL,             /* Parameter passed as input of the task */
                    1,                /* Priority of the task. */
                    NULL,             /* Task handle. */
                    0);               /* pin to core 0 */
    #endif

    xTaskCreatePinnedToCore(
                    displayResults,          /* Task function. */
                    "DisplayResults",        /* String with name of task. */
                    10000,            /* Stack size in bytes. */
                    NULL,             /* Parameter passed as input of the task */
                    1,                /* Priority of the task. */
                    NULL,             /* Task handle. */
                    1);               /* pin to core 1 */

    xTaskCreatePinnedToCore(
                    alarmTask,          /* Task function. */
                    "alarmTask",        /* String with name of task. */
                    10000,            /* Stack size in bytes. */
                    NULL,             /* Parameter passed as input of the task */
                    1,                /* Priority of the task. */
                    NULL,             /* Task handle. */
                    1);               /* pin to core 1 */
}

void loop() {
    delay(1000);
}


// monitor for alarms and flash the alarm if needed
void alarmTask( void * parameter) 
{   
    Alarm_t alarms;
    alarmMuteTimer = 0;
    unsigned long pulseTimer = 0;
    int alarmState = LOW;

    for (;;) {
        xQueuePeek(alarmQ, &alarms, 10);
        if ((millis() > alarmMuteTimer) && (alarms.pmax || alarms.pwarn || alarms.mvhi || alarms.mvlo || alarms.dc) && (millis() > pulseTimer)) {
            alarmState = !alarmState;
            digitalWrite(ALARM, alarmState);
            pulseTimer = millis() + 1000;
        } else {
            digitalWrite(ALARM, LOW);
        }
        delay(100);
    }
}

// read sensor
void readSensor( void * parameter )
{
    Sample_t sample;
    uint32_t ieTimer = 0;
    uint32_t breathTimer = 0;
    State_t state;
    Settings_t settings;
    Alarm_t alarms;
    float v;
    breathTimer = 0;
    ieTimer = 0;

    for(;;) {
        xQueuePeek(stateQ, &state, 10);
        xQueuePeek(settingQ, &settings, 10);
        xQueuePeek(alarmQ, &alarms, 10);

        // read patient pressure & timestamp
        psens.read();
        sample.p = psens.p;
        sample.p_ts = psens.t;

        if (psens.p > settings.pmax) {
            // exceeded hard pressure ceiling, retract piston, fire alarm
            digitalWrite(PISTON, LOW);
            alarms.pmax = true;
            alarms.pwarn = false;  // overwrite soft warning
            xQueueOverwrite(alarmQ, &alarms);
        } else if (psens.p > settings.peakAlarm) {
            // exceeded soft pressure warning threshold, fire alarm
            alarms.pwarn = true;
            xQueueOverwrite(alarmQ, &alarms);
        }

        switch(state.phase) {
            case EXPIRATORY:
                eFsens.read();
                iFsens.read();
                sample.f = iFsens.f - eFsens.f;
                sample.f_ts = eFsens.t;
                sample.v = v +iFsens.v - eFsens.v;
                sample.v_ts = eFsens.t;

                // if time for a new breath or if patient tried to inhale
                if (millis() > breathTimer || (settings.trig > 0 && iFsens.v > settings.trig))  {
                    state.phase = INSPIRATORY;
                    state.peep = psens.p;       // capture PEEP at end of expiratory phase
                    psens.peak = 0;             // reset peak pressure tracker
                    eFsens.v = 0;               // reset expiratory flow meter
                    xQueueOverwrite(stateQ, &state);

                    if (settings.power) {
                        digitalWrite(PISTON, HIGH);     // squeeze bag
                        digitalWrite(EXPVALVE, LOW);    // close expiratory path
                    }

                    // reset timer
                    breathTimer = round(60.0 / settings.rr * 1000.0) + millis();
                    ieTimer = round((60.0 / settings.rr * 1000.0) * (1.0 - (float(settings.ier)/10.0 / (float(settings.ier)/10.0+1.0)))) + millis();
                }
                break;
            default: // INSPIRATORY or POSTINSPIRATORY
                iFsens.read();
                // read inspiratory flow
                sample.f = iFsens.f;
                sample.f_ts = iFsens.t;
                // read inspiratory volume
                sample.v = iFsens.v;
                sample.v_ts = iFsens.t;

                // check if we've hit desired TV
                if (state.phase == INSPIRATORY && iFsens.v >= settings.tv) {
                    digitalWrite(PISTON, LOW);          // retract piston
                    state.phase = POSTINSPIRATORY;
                    xQueueOverwrite(stateQ, &state);
                }

                if (millis() > ieTimer) {
                    // retract piston, open expiratory valve
                    if (settings.power) {
                        digitalWrite(PISTON, LOW);
                        digitalWrite(EXPVALVE, HIGH);
                    }
                    iFsens.updateMv();          // update minute volume tracker
                    state.phase = EXPIRATORY;
                    state.peak = psens.peak;    // report peak pressure during inspiratory phase
                    state.plat = psens.avg;     // log avg of last 10 samples as plateau pressure
                    state.tv = iFsens.v;        // log tidal volume delivered during inspiratory phase
                    state.minvol = iFsens.mv;   // log minute volume
                    alarms.mvhi = (iFsens.mv / 1000.0 > settings.mvhiAlarm);    // set alarms for minute volume
                    alarms.mvlo = (iFsens.mv / 1000.0 < settings.mvloAlarm);
                    state.rr = iFsens.rr;       // log the actual respiratory rate
                    v = iFsens.v;               // save off delivered tidal volume
                    iFsens.v = 0;               // reset insp flow meter to look for patient trigger
                    xQueueOverwrite(stateQ, &state);
                    xQueueOverwrite(alarmQ, &alarms);
                }
                break;
        }
        if(xQueueSend(sampleQ, &sample, 0) != pdTRUE) {
            Serial.println("queue error\n\r");
        }
        delay(10);
    }
}



// print sensor results
void displayResults( void * parameter)
{
    Sample_t sample;
    Settings_t settings;
    State_t state;
    Alarm_t alarms;

    // track current screen and screen transitions
    Screen screen = SETSCREEN;
    Screen oldScreen = NOSCREEN;

    // track phase transitions
    Phase oldPhase = INSPIRATORY;

    // struct to pass values between mod screen and set screen
    ModVal_t modvals = {"", 0, 0, 0, 0};

    SetScreen setScreen = SetScreen(tft, screen, settingQ, modvals, alarmMuteTimer);
    MainScreen mainScreen = MainScreen(tft, screen);
    ModScreen modScreen = ModScreen(tft, screen, modvals);
    AlarmScreen alarmScreen = AlarmScreen(tft, screen, settingQ, modvals);

    for(;;) {
        // if there was a screen transition, draw the new screen
        if (oldScreen != screen) {
            switch(screen) {
                case SETSCREEN:
                    setScreen.draw();
                    break;
                case MAINSCREEN:
                    mainScreen.draw();
                    break;
                case MODSCREEN:
                    modScreen.draw();
                    break;
                case ALARMSCREEN:
                    alarmScreen.draw();
                    break;
            }
            oldScreen = screen;
        };

        // look for state changes
        if (xQueuePeek(stateQ, &state, 10) == pdTRUE) {
            // if there was a phase transition to expiratory, update measurements
            if (oldPhase != EXPIRATORY && state.phase == EXPIRATORY) {
                if (screen == MAINSCREEN) {
                    mainScreen.updateMeas(state);
                }
            }
            oldPhase = state.phase;
        };

        // pull samples off the sample queue
        // need to be sure we do this fast enough to not stall sample queue
        if(xQueueReceive(sampleQ, &sample,100) == pdTRUE) {
            // if we're on the main screen plot the sample, otherwise ignore it
            if (screen == MAINSCREEN) {
                mainScreen.update(sample);
            }
        }

        // read alarm state and write alarms to screen
        if(xQueuePeek(alarmQ, &alarms, 10) == pdTRUE) {
            if (screen == MAINSCREEN) {
                mainScreen.updateAlarms(alarms);
            }
        }

        // Touch Handler
        // touchscreen library expects 10-bit voltage measurements
        // if using analog sensors, make sure to set to 12-bit before reading analog sensors
        analogReadResolution(10);
        TSPoint p = ts.getPoint();
        // Scale from ~0->1000 to tft.width using the calibration #'s
        int y = map(p.x, TS_MINX, TS_MAXX, 0, tft.height());
        int x = map(p.y, TS_MINY, TS_MAXY, tft.width(), 0);
        p.x = x;
        p.y = y;

        if (p.z > MINPRESSURE && p.z < MAXPRESSURE) {
            switch(screen) {
                case MAINSCREEN:
                    mainScreen.handleTouch(p);
                    break;
                case SETSCREEN:
                    setScreen.handleTouch(p);
                    break;
                case MODSCREEN:
                    modScreen.handleTouch(p);
                    break;
                case ALARMSCREEN:
                    alarmScreen.handleTouch(p);
                    break;
            }
        }
        delay(1);
    }
}

#ifdef SIM_LUNG
// task to simulate a lung in simulation mode
void simLung(void * parameter) {
    Lung lung = Lung(stateQ, lungQ);
    for(;;) {
        lung.update();
        delay(1);
    }
}
#endif

// initialize the queues
void initQ() {
    sampleQ = xQueueCreate(1000, sizeof(Sample_t));
    settingQ = xQueueCreate(1, sizeof(Settings_t));
    stateQ = xQueueCreate(1, sizeof(State_t));
    alarmQ = xQueueCreate(1, sizeof(Alarm_t));

    Settings_t init = {
        15,     // rr
        10,     // ier
        40,     // pmax
        30,     // peakAlarm
        20,     // mvhiAlarm
        0,      // mvloAlarm
        3,      // dcAlarm
        300,    // tv
        50,     // trigger
        false   // power
    };
    xQueueOverwrite(settingQ, &init);

    State_t state = {
        0,      // rr
        0,      // peep
        0,      // peak pressure
        0,      // plat pressure
        0,      // tv
        0,      // minute volume
        EXPIRATORY
    };
    xQueueOverwrite(stateQ, &state);

    Alarm_t alarms = {
        false, false, false, false, false
    };
    xQueueOverwrite(alarmQ, &alarms);
}