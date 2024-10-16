#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>
#include "clock.c"
#include "colors.h"

#define BUF_SIZE 128
#define BAT_SIZE 256

char *statuses[] = { "Unknown", "Charging", "Discharging", "Not charging", "Full" };
enum Status {
    UNKNOWN,
    CHARGING,
    DISCHARGING,
    NOT_CHARGING,
    FULL
};

struct PSData {
    enum Status status;
    int voltageNow;
    int powerNow;
    int currentNow;
    int energyFull;
    int energyNow;
    double capacity;
};

int maxInt(int a, int b) {
    return a > b ? a : b;
}

int strWidth(char *str) {
    int extraCount = 0;
    bool escaped = false;
    for (int i = 0; i < strlen(str); i++) {
        if (str[i] == '\033') {
            escaped = true;
        } 
        if (escaped == true) {
            extraCount++;
        }
        if (str[i] == 'm') {
            escaped = false;
        }
    }
    return strlen(str) - extraCount;
}

void psInit(struct PSData *ps) {
    FILE *fptr;
    fptr = fopen("/sys/class/power_supply/BAT0/uevent", "r");
    if (fptr == NULL) {
        printf("File not opened");
        exit(0);
    }

    char *subStr;
    char line[BUF_SIZE];
    char statusStr[BUF_SIZE];

    while (fgets(line, BUF_SIZE, fptr)) {
        subStr = strtok(line, "=\n");
        if (strcmp(subStr, "POWER_SUPPLY_STATUS") == 0) {
            strncpy(statusStr, strtok(NULL, "=\n"), BUF_SIZE);
        } else if (strcmp(subStr, "POWER_SUPPLY_VOLTAGE_NOW") == 0) {
            ps->voltageNow = atoi(strtok(NULL, "=\n"));
        } else if (strcmp(subStr, "POWER_SUPPLY_POWER_NOW") == 0) {
            ps->powerNow = atoi(strtok(NULL, "=\n"));
        } else if (strcmp(subStr, "POWER_SUPPLY_ENERGY_FULL") == 0) {
            ps->energyFull = atoi(strtok(NULL, "=\n"));
        } else if (strcmp(subStr, "POWER_SUPPLY_ENERGY_NOW") == 0) {
            ps->energyNow = atoi(strtok(NULL, "=\n"));
        } 
    }
    for (int i = 0; i <= FULL; i++) {
        if (strcmp(statusStr, statuses[i]) == 0) {
            ps->status = i;
            break;
        }
    }
    ps->capacity = (double) 100 * ps->energyNow / ps->energyFull;
    ps->currentNow = (double) 1000000 * ps->powerNow / ps->voltageNow;
    fclose(fptr);
}


char *genBatGraphic(struct PSData *ps) {
    char *batGraphic = malloc(BAT_SIZE * sizeof(char));
    const char *batColor;
    if (ps->capacity > 60) {
        batColor = clrs[G];
    } else if (ps->capacity > 25) {
        batColor = clrs[Y];
    } else {
        batColor = clrs[R];
    }

    double timeLeft;
    if (ps->status == CHARGING) {
        timeLeft = (double) (ps->energyFull - ps->energyNow) / ps->powerNow;
    } else if (ps->status == DISCHARGING || ps->status == NOT_CHARGING) {
        timeLeft = (double) ps->energyNow / ps->powerNow;
    }
    int hrs = floor(timeLeft);
    int mins = 60 * (timeLeft - floor(timeLeft));

    snprintf(batGraphic, BAT_SIZE, "%sbat:\n%s", clrs[C], clrs[Z]);

    strlcatf(batGraphic, BAT_SIZE, "[%s", batColor);
    int pipeNum = round((double) ps->capacity / 5);
    for (int i = 0; i < pipeNum; i++) {
        strlcatf(batGraphic, BAT_SIZE, "|");
    }
    strlcatf(batGraphic, BAT_SIZE, "%s", clrs[Z]);
    for (int i = 0; i < 20 - pipeNum; i++) {
        strlcatf(batGraphic, BAT_SIZE, "-");
    }
    strlcatf(batGraphic, BAT_SIZE, "]\n");

    strlcatf(batGraphic, BAT_SIZE, "%s%.1f%% %s(%s)\n%s", clrs[C], ps->capacity, clrs[BK], statuses[ps->status], clrs[BK]);

    if (ps->status != FULL && ps->status != UNKNOWN) {
        strlcatf(batGraphic, BAT_SIZE, "%02d:%02d until %s\n%s", hrs, mins, ps->status == CHARGING ? "full" : "depleted", clrs[BK]);
    }
    strlcatf(batGraphic, BAT_SIZE, "%.2fW, ", (double) ps->powerNow / 1000000);
    strlcatf(batGraphic, BAT_SIZE, "%.2fV, ", (double) ps->voltageNow / 1000000);
    strlcatf(batGraphic, BAT_SIZE, "%.2fA\n", (double) ps->currentNow / 1000000);
    
    return batGraphic;
}

void printGraphics(char** graphics, int len, int padding) {
    int maxLineWidths[len];
    int lineCounts[len];
    int maxLineCount = 0;

    for (int i = 0; i < len; i++) {
        int curMax = 0;
        int lineCount = 1;
        char *curLine = strtok(graphics[i], "\n");
        while ((curLine = strtok(NULL, "\n")) != NULL) {
            curMax = maxInt(curMax, strWidth(curLine));
            lineCount++;
        }
        maxLineWidths[i] = curMax;
        lineCounts[i] = lineCount;
        maxLineCount = maxInt(maxLineCount, lineCount);
    }

    for (int j = 0; j < maxLineCount; j++) {
        for (int i = 0; i < len; i++) {
            int curWidth = strWidth(graphics[i]);
            if (j < maxLineCount - lineCounts[i]) {
                printf("%*s", maxLineWidths[i] + padding, "");
                continue;
            }
            if (graphics[i][0] != '\0') {
                printf("%s", graphics[i]);
                graphics[i] += strlen(graphics[i]) + 1;
            }
            printf("%s%*s", clrs[Z], maxLineWidths[i] - curWidth + padding, "");
        }

        printf("\n");
    }
}


int main() {
    //system("clear");

    time_t t = time(NULL);
    struct tm curTime = *localtime(&t);
    char *clockGraphic = genClockGraphic(curTime.tm_hour, curTime.tm_min, curTime.tm_sec);
    char *clock7_35_28 = genClockGraphic(7, 05, 28);

    struct PSData supplyData;
    psInit(&supplyData);
    char *batGraphic = genBatGraphic(&supplyData);

    char *graphics[] = { clockGraphic, batGraphic };
    printGraphics(graphics, sizeof(graphics) / sizeof(graphics[0]), 4);

    free(clockGraphic);
    free(batGraphic);
    free(clock7_35_28);
    printf("\n");

    
    return 0;
}
