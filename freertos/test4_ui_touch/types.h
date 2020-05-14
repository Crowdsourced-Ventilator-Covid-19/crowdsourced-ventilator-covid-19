#ifndef types_h
#define types_h

typedef struct {
    uint32_t t;
    float v;
} Sample_t;

typedef struct {
    int rr;
    float ier;
    int pmax;
    int peakAlarm;
    int mvhiAlarm;
    int mvloAlarm;
    int dcAlarm;
    int tv;
    int trig;
    bool power;
} Settings_t;

enum Screen {
    NOSCREEN,
    SETSCREEN, 
    MAINSCREEN, 
    MODSCREEN, 
    ALARMSCREEN 
};

enum Phase {
    NOPHASE,
    INSPIRATORY,
    EXPIRATORY
};

typedef struct {
    float rr;
    float peep;
    float peak;
    float plat;
    float tv;
    float poff;
    float foff;
    float minvol;
    Phase phase;
} State_t;


#endif