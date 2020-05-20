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
} Settings_t;

typedef struct {
    float peep;
    float peak;
    float plat;
    float tv;
    float poff;
    float foff;
} State_t;

#endif