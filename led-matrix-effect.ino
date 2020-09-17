#include <FastLED.h>

#define NUMLEDS 15
#define LEDPIN 23

#define NUMCOLS 2
#define LEDSPERCOL 7

#define GREEN CHSV(100, 255, 255)
#define BLACK CRGB(0, 0, 0)

extern const uint8_t gammaVals[];

CRGB leds[NUMLEDS];

uint8_t curStep = 0;

uint8_t lengths[NUMCOLS];

void setup() {
    Serial.begin(115200);

    FastLED.addLeds<NEOPIXEL, LEDPIN>(leds, NUMLEDS);

    FastLED.setDither(0);

    FastLED.setBrightness(pgm_read_byte(&gammaVals[127]));

    random16_add_entropy(analogRead(A0));

    // generate random length for each col
    for (uint8_t i = 0; i < NUMCOLS; i++) {
        lengths[i] = random8(2, 5);
    }
}

void loop() {

    // uint8_t head = 0;

    // while (head < 4) {
    //     fill_solid(&(leds[0]), NUMLEDS, CRGB(0, 0, 0));
    //     leds[head] = GREEN;
    //     FastLED.delay(300);
    //     head++;
    // }

    // nextStep();
    // update();
    // FastLED.delay(5);

    // fill_solid(leds, NUMLEDS, BLACK);
    // // leds[14] = GREEN;
    // FastLED.show();
    // FastLED.delay(100);

    // uint8_t length = 4;

    // while (length > 0) {
    //     shiftSection(leds, NUMLEDS, GREEN);
    //     FastLED.show();
    //     FastLED.delay(100);
    //     length--;
    // }

    // for (int i = 0; i < NUMLEDS; i++) {
    //     shiftSection(leds, NUMLEDS);
    //     FastLED.show();
    //     delay(100);
    // }

    shiftAll();
    refreshLengths();

    FastLED.show();
    FastLED.delay(100);

    // while (!areAllPixelsOff()) {
    //     shiftAll();
    //     refreshLengths();

    //     FastLED.show();
    //     FastLED.delay(100);
    // }
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
                lengths[i] = random8(2, 5);
            }
        }
    }
}

uint8_t areAllPixelsInSectionOff(CRGB *ledsP, uint8_t numLeds) {
    uint8_t allOff = true;
    for (uint8_t i = 0; i < numLeds; i++) {
        if (*(ledsP + i)) {
            allOff = false;
        }
    }
    return allOff;
}

uint8_t areAllPixelsOff() {
    uint8_t allOff = true;
    for (uint8_t i = 0; i < NUMLEDS; i++) {
        if (leds[i]) {
            allOff = false;
        }
    }
    return allOff;
}

void nextStep() {
    // for (uint8_t i = 0; i < NUMCOLS; i++) {
    //     if (heads[i] < LEDSPERCOL) {
    //         heads[i]++;
    //     } else {
    //         uint8_t random = random8();
    //         if (random < 127) {
    //             heads[i] = 0;
    //         }
    //     }
    // }

    // for (uint8_t led = 0; led < NUMLEDS; led++) {
    //     uint16_t offset = map(led, 0, NUMLEDS, 0, 255);
    //     uint8_t ledOffset = offset + curStep;
    //     if (ledOffset > 127) {
    //         uint8_t val = cubicwave8(ledOffset);
    //         leds[led] = CHSV(100, 255, pgm_read_byte(&gammaVals[val]));
    //     } else {
    //         leds[led] = BLACK;
    //     }
    // }

    waveUpdateSection(leds, 5);

    FastLED.show();

    curStep++;
}

void waveUpdateSection(CRGB *ledsP, uint8_t numLeds) {
    for (uint8_t led = 0; led < numLeds; led++) {
        uint8_t ledOffset = map(led, 0, numLeds, 0, 255) + curStep;
        if (ledOffset > 127) {
            uint8_t val = cubicwave8(ledOffset);
            *(ledsP + led) = CHSV(100, 255, pgm_read_byte(&gammaVals[val]));
        } else {
            *(ledsP + led) = BLACK;
        }
    }
}

void shiftSection(CRGB *ledsP, uint8_t numLeds, CRGB underflowColor) {
    for (uint8_t led = 0; led < numLeds - 1; led++) {
        *(ledsP + led) = *(ledsP + led + 1);
    }
    *(ledsP + numLeds - 1) = underflowColor;
}

// void shiftSection(CRGB *ledsP, uint8_t numLeds, CHSV underflowColor) {
//     for (uint8_t led = 0; led < numLeds - 1; led++) {
//         *(ledsP + led) = *(ledsP + led + 1);
//     }
//     *(ledsP + numLeds - 1) = underflowColor;
// }

void shiftSection(CRGB *ledsP, uint8_t numLeds) {
    shiftSection(ledsP, numLeds, BLACK);
}

void update() {
    // fill_solid(&(leds[0]), NUMLEDS, CRGB(0, 0, 0));
    // for (uint8_t i = 0; i < NUMCOLS; i++) {
    //     if (heads[i] < LEDSPERCOL + 2) {
    //         uint8_t headLed = LEDSPERCOL * i + heads[i];
    //         leds[headLed] = GREEN;
    //         if (heads[i] > 0) {
    //             leds[headLed - 1] = GREEN;
    //             leds[headLed - 1] %= pgm_read_byte(&gammaVals[172]);
    //         }
    //         if (heads[i] > 1) {
    //             leds[headLed - 2] = GREEN;
    //             leds[headLed - 2] %= pgm_read_byte(&gammaVals[64]);
    //         }
    //     }
    // }
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