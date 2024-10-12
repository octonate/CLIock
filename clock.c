#include <stdio.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include "clock.h"

void flipHor(char str[CLOCK_BUF_SIZE]) {
    char flipped[CLOCK_BUF_SIZE];
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
    strncpy(str, flipped, CLOCK_BUF_SIZE);
}

void clockAddHand(char dest[CLOCK_BUF_SIZE], const char hand[CLOCK_BUF_SIZE]) {
    for (int i = 0; i < CLOCK_BUF_SIZE; i++) {
        if (hand[i] != '.' && hand[i] != ' ') {
            dest[i] = hand[i];
        }
    }
}

void printClock() {
    char out[CLOCK_BUF_SIZE];
    char hrHand[CLOCK_BUF_SIZE];
    char minHand[CLOCK_BUF_SIZE];
    strncpy(out, clockFrame, CLOCK_BUF_SIZE);
    strncpy(hrHand, clockBlank, CLOCK_BUF_SIZE);
    strncpy(minHand, clockBlank, CLOCK_BUF_SIZE);

    time_t t = time(NULL);
    struct tm curTime = *localtime(&t);
    int hrs = curTime.tm_hour % 12;
    int mins = curTime.tm_min;
    int secs = curTime.tm_sec;
    int minHandPos = (int) round((double) (60 * mins + secs) / 150) %  24;
    

    if (minHandPos <= 12) {
        clockAddHand(minHand, minHands[minHandPos]);
    } else {
        clockAddHand(minHand, minHands[24 - minHandPos]);
        flipHor(minHand);
    }

    if (hrs <= 6) {
        clockAddHand(hrHand, hrHands[hrs]);
    } else {
        clockAddHand(hrHand, hrHands[12 - hrs]);
        flipHor(hrHand);
    }

    clockAddHand(out, minHand);
    clockAddHand(out, hrHand);
    
    printf("%s\n", out);
    printf("%s", ctime(&t));
}
