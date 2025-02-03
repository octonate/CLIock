#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>
#include "clock.c"
#include "clock.h"
#include "colors.h"

#define BUF_SIZE 128
#define BAT_SIZE 256

char *statuses[] = { "Unknown", "Charging", "Discharging", "Not charging", "Full", "NULLVAL" };
enum Status {
    UNKNOWN,
    CHARGING,
    DISCHARGING,
    NOT_CHARGING,
    FULL,
};

struct PSData {
    enum Status status;
    unsigned int voltageNow;
    int powerNow;
    int currentNow;
    int energyFull;
    int energyNow;
    double capacity;
};

struct Flags {
    bool battery;
    bool clock;
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


void genBatGraphic(struct PSData *ps, char batGraphic[BAT_SIZE]) {
    enum Color batClr;
    if (ps->capacity > 60) {
        batClr = GREEN;
    } else if (ps->capacity > 25) {
        batClr = YELLOW;
    } else {
        batClr = RED;
    }

    double timeLeft;
    if (ps->status == CHARGING) {
        timeLeft = (double) (ps->energyFull - ps->energyNow) / ps->powerNow;
    } else if (ps->status == DISCHARGING) {
        timeLeft = (double) ps->energyNow / ps->powerNow;
    }
    int hrs = floor(timeLeft);
    int mins = 60 * (timeLeft - floor(timeLeft));

    snprintf(batGraphic, BAT_SIZE, "%sbat:\n%s", ansiClrs[highlightTextColor], ansiClrs[DEFAULT]);

    strlcatf(batGraphic, BAT_SIZE, "[%s", ansiClrs[batClr]);
    int pipeNum = round((double) ps->capacity / 5);
    for (int i = 0; i < pipeNum; i++) {
        strlcatf(batGraphic, BAT_SIZE, "|");
    }
    strlcatf(batGraphic, BAT_SIZE, "%s", ansiClrs[DEFAULT]);
    for (int i = 0; i < 20 - pipeNum; i++) {
        strlcatf(batGraphic, BAT_SIZE, "-");
    }
    strlcatf(batGraphic, BAT_SIZE, "]\n");

    strlcatf(batGraphic, BAT_SIZE, "%s%.1f%% %s(%s)\n%s", ansiClrs[highlightTextColor], ps->capacity, ansiClrs[regularTextColor], statuses[ps->status], ansiClrs[regularTextColor]);

    if (ps->status == CHARGING || ps->status == DISCHARGING) {
        strlcatf(batGraphic, BAT_SIZE, "%02d:%02d until %s\n%s", hrs, mins, ps->status == CHARGING ? "full" : "depleted", ansiClrs[regularTextColor]);
    }
    strlcatf(batGraphic, BAT_SIZE, "%.2fW, ", (double) ps->powerNow / 1000000);
    strlcatf(batGraphic, BAT_SIZE, "%.2fV, ", (double) ps->voltageNow / 1000000);
    strlcatf(batGraphic, BAT_SIZE, "%.2fA\n", (double) ps->currentNow / 1000000);
}

void printGraphics(char **graphics, int len, int padding) {
    int maxLineWidths[len];
    int lineCounts[len];
    memset(maxLineWidths, 0, sizeof(maxLineWidths));
    memset(lineCounts, 0, sizeof(lineCounts));
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
            printf("%s%*s", ansiClrs[DEFAULT], maxLineWidths[i] - curWidth + padding, "");
        }

        printf("\n");
    }
}


int main(int argc, char *argv[]) {
    system("clear");

    char clockGraphic[CLOCK_SIZE] = {0};
    char batGraphic[BAT_SIZE];
    char *graphics[2];

    struct Flags flags = { .battery = false, .clock = true };
    int graphicsLen = 1;
    for (int i = 0; i < argc; i++) {
        if (!flags.battery && (strncmp(argv[i], "-b", BUF_SIZE) == 0 || strncmp(argv[i], "--battery", BUF_SIZE) == 0)) {
            graphicsLen++;
            flags.battery = true;
        }
        if (flags.clock && (strncmp(argv[i], "-nc", BUF_SIZE) == 0 || strncmp(argv[i], "--no-clock", BUF_SIZE) == 0)) {
            flags.clock = false;
            graphicsLen--;
        }
    }

    if (flags.clock) {
        time_t t = time(NULL);
        struct tm curTime = *localtime(&t);
        genClockGraphic(curTime.tm_hour, curTime.tm_min, curTime.tm_sec, clockGraphic);
        //genClockGraphic(03, 35, 42, clockGraphic);
        graphics[0] = clockGraphic;
    }

    if (flags.battery) {
        struct PSData supplyData;
        psInit(&supplyData);
        genBatGraphic(&supplyData, batGraphic);
        graphics[graphicsLen - 1] = batGraphic;
    }
    
    printGraphics(graphics, graphicsLen, 4);

    printf("\n");
    
    return 0;
}
