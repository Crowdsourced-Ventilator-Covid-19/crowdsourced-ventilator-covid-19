#ifndef TYPES_H
#define TYPES_H

typedef struct {
    float p;
    uint32_t p_ts;
    float v;
    uint32_t v_ts;
    float f;
    uint32_t f_ts;
} Sample_t;

typedef struct {
    int rr;
    int ier;
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
    POSTINSPIRATORY,
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

typedef struct {
    String label;
    int minv;
    int maxv;
    int val;
    int newval;
} ModVal_t;

typedef struct {
    float p;
    float fin;
    float fout;
} Lung_t;

#endif