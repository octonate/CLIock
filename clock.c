#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include "clock.h"

void flipHor(char str[CLOCK_GRAPHIC_SIZE]) {
    char flipped[CLOCK_GRAPHIC_SIZE];
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
    strncpy(str, flipped, CLOCK_GRAPHIC_SIZE);
}

void clockAddHand(char dest[CLOCK_GRAPHIC_SIZE], const char hand[CLOCK_GRAPHIC_SIZE]) {
    for (int i = 0; i < CLOCK_GRAPHIC_SIZE; i++) {
        if (hand[i] != '.' && hand[i] != ' ') {
            dest[i] = hand[i];
        }
    }
}

char *genClockGraphic(int hrs, int mins, int secs) {
    int hrsStd = hrs % 12;
    char *clockDisp = malloc(CLOCK_GRAPHIC_SIZE * sizeof(char));
    char hrHand[CLOCK_GRAPHIC_SIZE];
    char minHand[CLOCK_GRAPHIC_SIZE];
    strncpy(clockDisp, clockFrame, CLOCK_GRAPHIC_SIZE);
    strncpy(hrHand, clockBlank, CLOCK_GRAPHIC_SIZE);
    strncpy(minHand, clockBlank, CLOCK_GRAPHIC_SIZE);

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
    
    snprintf(clockDisp + strlen(clockDisp), CLOCK_GRAPHIC_SIZE, "%02d:%02d:%02d\n", hrs, mins, secs);
    return clockDisp;
}
