#include <FastLED.h>

#define LED_TYPE WS2812B

#define LEDPIN0 17
#define LEDPIN1 16
#define LEDPIN2 19
#define LEDPIN3 18
#define LEDPIN4 5

#define NUMCOLS 5
#define LEDSPERCOL 5

#define TOTALLEDS (NUMCOLS * LEDSPERCOL)

#define BRIGHTNESS 64

#define FRAMEDELAY 100

#define GREEN CHSV(96, 255, 255)
#define BLACK CRGB(0, 0, 0)

extern const uint8_t gammaVals[];

CRGB leds[TOTALLEDS];

uint8_t lengths[NUMCOLS];

void setup() {
    // add each strip into leds array
    FastLED.addLeds<LED_TYPE, LEDPIN0, GRB>(leds, LEDSPERCOL);
    FastLED.addLeds<LED_TYPE, LEDPIN1, GRB>(leds, LEDSPERCOL, LEDSPERCOL);
    FastLED.addLeds<LED_TYPE, LEDPIN2, GRB>(leds, LEDSPERCOL * 2, LEDSPERCOL);
    FastLED.addLeds<LED_TYPE, LEDPIN3, GRB>(leds, LEDSPERCOL * 3, LEDSPERCOL);
    FastLED.addLeds<LED_TYPE, LEDPIN4, GRB>(leds, LEDSPERCOL * 4, LEDSPERCOL);

    FastLED.setDither(0);

    FastLED.setBrightness(pgm_read_byte(&gammaVals[BRIGHTNESS]));

    random16_add_entropy(analogRead(A0));

    // generate random length for each col
    for (uint8_t i = 0; i < NUMCOLS; i++) {
        lengths[i] = getRandomLength();
    }
}

void loop() {
    shiftAll();
    refreshLengths();

    FastLED.show();
    FastLED.delay(FRAMEDELAY);
}

void shiftAll() {
    for (uint8_t i = 0; i < NUMCOLS; i++) {
        if (lengths[i] > 0) {
            shiftSection(&leds[LEDSPERCOL * i], LEDSPERCOL, GREEN);
            lengths[i]--;
        } else {
            shiftSection(&leds[LEDSPERCOL * i], LEDSPERCOL, BLACK);
        }
    }
}

void refreshLengths() {
    for (uint8_t i = 0; i < NUMCOLS; i++) {
        if (lengths[i] == 0 && areAllPixelsInSectionOff(&leds[LEDSPERCOL * i], LEDSPERCOL)) {
            uint8_t random = random8();
            if (random < 128) {
                lengths[i] = getRandomLength();
            }
        }
    }
}

uint8_t getRandomLength() {
    return random8(2, 6);
}

void shiftSection(CRGB *ledsP, uint8_t numLeds, CRGB underflowColor) {
    for (uint8_t led = numLeds - 1; led > 0; led--) {
        *(ledsP + led) = *(ledsP + led - 1);
    }
    *(ledsP) = underflowColor;
}

void shiftSection(CRGB *ledsP, uint8_t numLeds) {
    shiftSection(ledsP, numLeds, BLACK);
}

const PROGMEM uint8_t gammaVals[] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2,
    2, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 5, 5, 5,
    5, 6, 6, 6, 6, 7, 7, 7, 7, 8, 8, 8, 9, 9, 9, 10,
    10, 10, 11, 11, 11, 12, 12, 13, 13, 13, 14, 14, 15, 15, 16, 16,
    17, 17, 18, 18, 19, 19, 20, 20, 21, 21, 22, 22, 23, 24, 24, 25,
    25, 26, 27, 27, 28, 29, 29, 30, 31, 32, 32, 33, 34, 35, 35, 36,
    37, 38, 39, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 50,
    51, 52, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 66, 67, 68,
    69, 70, 72, 73, 74, 75, 77, 78, 79, 81, 82, 83, 85, 86, 87, 89,
    90, 92, 93, 95, 96, 98, 99, 101, 102, 104, 105, 107, 109, 110, 112, 114,
    115, 117, 119, 120, 122, 124, 126, 127, 129, 131, 133, 135, 137, 138, 140, 142,
    144, 146, 148, 150, 152, 154, 156, 158, 160, 162, 164, 167, 169, 171, 173, 175,
    177, 180, 182, 184, 186, 189, 191, 193, 196, 198, 200, 203, 205, 208, 210, 213,
    215, 218, 220, 223, 225, 228, 231, 233, 236, 239, 241, 244, 247, 249, 252, 255};