#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <stdarg.h>
#include "clock.h"

size_t strlcatf(char *dst, size_t size, const char *fmt, ...) {
    va_list args;
    size_t dstLen = strnlen(dst, size);
    char *fmtPos = dst + strlen(dst);
    va_start(args, fmt);
    size_t fmtLen = vsnprintf(fmtPos, size - strlen(dst), fmt, args);
    va_end(args);
    return dstLen + fmtLen;
}

void flipHor(char str[CLOCK_SIZE]) {
    char flipped[CLOCK_SIZE];
    int pos = 0;
    char *tok = strtok(str, "\n");
    while (tok != NULL) {
        int len = strlen(tok);
        for (int i = len - 1; i >= 0; i--) {
            switch (tok[len - i - 1]) {
            case '/':
                flipped[pos + i]  = '\\';
                break;
            case '\\':
                flipped[pos + i]  = '/';
                break;
            case '>':
                flipped[pos + i]  = '<';
                break;
            case '<':
                flipped[pos + i]  = '>';
                break;
            default:
                flipped[pos + i] = tok[len - i - 1];
                break;
            }
        }
        flipped[pos + len] = '\n'; 
        pos += len + 1;
        tok = strtok(NULL, "\n");
    }
    flipped[pos] = '\0';
    strncpy(str, flipped, CLOCK_SIZE);
}

void clockAddHand(char dest[CLOCK_SIZE], const char hand[CLOCK_SIZE]) {
    for (int i = 0; i < CLOCK_SIZE; i++) {
        if (hand[i] != '.' && hand[i] != ' ') {
            dest[i] = hand[i];
        }
    }
}

char *genClockGraphic(int hrs, int mins, int secs) {
    char *clockDisp = malloc(CLOCK_SIZE * sizeof(char));
    int hrsStd = hrs % 12;
    char hrHand[CLOCK_SIZE];
    char minHand[CLOCK_SIZE];
    strncpy(clockDisp, clockFrame, CLOCK_SIZE);
    strncpy(hrHand, clockBlank, CLOCK_SIZE);
    strncpy(minHand, clockBlank, CLOCK_SIZE);

    int minHandPos = (int) round((double) (60 * mins + secs) / 150) %  24;
    

    if (minHandPos <= 12) {
        clockAddHand(minHand, minHands[minHandPos]);
    } else {
        clockAddHand(minHand, minHands[24 - minHandPos]);
        flipHor(minHand);
    }

    if (hrsStd <= 6) {
        clockAddHand(hrHand, hrHands[hrsStd]);
    } else {
        clockAddHand(hrHand, hrHands[12 - hrsStd]);
        flipHor(hrHand);
    }

    clockAddHand(clockDisp, minHand);
    clockAddHand(clockDisp, hrHand);
    
    strlcatf(clockDisp, CLOCK_SIZE, "%02d:%02d:%02d\n", hrs, mins, secs);
    return clockDisp;
}
