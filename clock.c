#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <stdarg.h>
#include "clock.h"
#include "colors.h"



size_t strlcatf(char *dst, size_t size, const char *fmt, ...) {
    va_list args;
    size_t dstLen = strlen(dst);
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

void genClockGraphic(int hrs, int mins, int secs, char clockDisp[CLOCK_SIZE]) {
    char hrHand[CLOCK_SIZE];
    char minHand[CLOCK_SIZE];

    int hrHandPos = (hrs % 12 + (double) mins / 60 + (double) secs / 3600) * 2;
    int minHandPos = (int) round((double) (60 * mins + secs) / 150) %  24;
    
    if (minHandPos <= 12) {
        strncpy(minHand, minHands[minHandPos], CLOCK_SIZE);
    } else {
        strncpy(minHand, minHands[24 - minHandPos], CLOCK_SIZE);
        flipHor(minHand);
    }

    if (hrHandPos <= 12) {
        strncpy(hrHand, hrHands[hrHandPos], CLOCK_SIZE);
    } else {
        strncpy(hrHand, hrHands[24 - hrHandPos], CLOCK_SIZE);
        flipHor(hrHand);
    }

    
    for (size_t i = 0; i < strlen(hrHand); i++) {
        if (clockFrame[i] == 'o') {
            strlcatf(clockDisp, CLOCK_SIZE, "%s%c", ansiClrs[clockCenterColor], clockFrame[i]);
        } else if (hrHand[i] != ' ') {
            strlcatf(clockDisp, CLOCK_SIZE, "%s%c", ansiClrs[clockHourHandColor], hrHand[i]);
        } else if (minHand[i] != ' ') {
            strlcatf(clockDisp, CLOCK_SIZE, "%s%c", ansiClrs[clockMinHandColor], minHand[i]);
        } else if (clockTicks[i] != ' ') {
            strlcatf(clockDisp, CLOCK_SIZE, "%s%c", ansiClrs[clockTickMarkColor], clockTicks[i]);
        } else {
            strlcatf(clockDisp, CLOCK_SIZE, "%s%c", ansiClrs[clockFrameColor], clockFrame[i]);
        } 
    }

    strlcatf(clockDisp, CLOCK_SIZE, "%stime: %s%02d:%02d:%02d\n", ansiClrs[highlightTextColor], ansiClrs[regularTextColor], hrs, mins, secs);
}
