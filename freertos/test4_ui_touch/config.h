#ifndef config_h
#define config_h

#define TV_MIN     100
#define TV_MAX     800
#define RR_MIN     10
#define RR_MAX     40
#define IE_MAX     3
#define IE_MIN     1
#define PIP_MAX    60
#define PIP_MIN    10
#define TRIG_MAX   100
#define TRIG_MIN   0
#define PIP_ALRM_MAX 700
#define PIP_ALRM_MIN 100
#define MVHI_ALRM_MAX 30
#define MVHI_ALRM_MIN 0
#define MVLO_ALRM_MAX 30
#define MVLO_ALRM_MIN 0
#define DC_ALRM_MAX 20
#define DC_ALRM_MIN 0


// These are the four touchscreen analog pins
#define YP A10  // must be an analog pin, use "An" notation!
#define XM A13  // must be an analog pin, use "An" notation!
#define YM 33    // can be a digital pin
#define XP 25   // can be a digital pin

// This is calibration data for the raw touch data to the screen coordinates
#define TS_MINX 120
#define TS_MINY 90
#define TS_MAXX 940
#define TS_MAXY 959

#define MINPRESSURE 10
#define MAXPRESSURE 1000

// The display uses hardware SPI, plus #9 & #10
#define TFT_RST -1  // dont use a reset pin, tie to arduino RST if you like
#define TFT_DC 17
#define TFT_CS 5

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


#define flowDeadZone = 3
#endif