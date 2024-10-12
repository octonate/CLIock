#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "clock.c"

#define BUF_SIZE 128

char *clrs[] = { "\e[0m", "\e[30m", "\e[31m", "\e[32m", "\e[33m", "\e[34m", "\e[35m", "\e[36m", "\e[37m", "\e[90m", "\e[91m", "\e[92m", "\e[93m", "\e[94m", "\e[95m", "\e[96m", "\e[97m" };
enum { Z, K, R, G, Y, B, M, C, W, BK, BR, BG, BY, BB, BM, BC, BW };

char *statuses[] = { "Charging", "Discharging", "Full" };
enum Status {
    CHARGING,
    DISCHARGING,
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

void psInit(struct PSData *ps, FILE *fp) {
    char *subStr;
    char line[BUF_SIZE];
    char statusStr[BUF_SIZE];

    while (fgets(line, BUF_SIZE, fp)) {
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
}

void printBattery(struct PSData *ps) {
    char *batColor;
    if (ps->capacity > 60) {
        batColor = clrs[G];
    } else if (ps->capacity > 25) {
        batColor = clrs[Y];
    } else {
        batColor = clrs[R];
    }

    int width = round((double) ps->capacity / 5);
    char batDisp[BUF_SIZE];

    snprintf(batDisp, BUF_SIZE, "[%s", batColor);
    for (int i = 0; i < width; i++) {
        snprintf(batDisp + strlen(batDisp), BUF_SIZE, "|");
    }
    snprintf(batDisp + strlen(batDisp), BUF_SIZE, "%s", clrs[Z]);
    for (int i = 0; i < 20 - width; i++) {
        snprintf(batDisp + strlen(batDisp), BUF_SIZE, "-");
    }
    snprintf(batDisp + strlen(batDisp), BUF_SIZE, "]\n");

    double timeLeft;
    if (ps->status == CHARGING) {
        timeLeft = (double) (ps->energyFull - ps->energyNow) / ps->powerNow;
    } else if (ps->status == DISCHARGING) {
        timeLeft = (double) ps->energyNow / ps->powerNow;
    }
    int hrs = floor(timeLeft);
    int mins = 60 * (timeLeft - floor(timeLeft));


    printf("\n%sbat:%s\n", clrs[C], clrs[Z]);
    printf("%s", batDisp);
    printf("%s", clrs[C]);
    printf("%.1f%% %s(%s)\n", ps->capacity, clrs[BK], statuses[ps->status]);
    if (ps->status != FULL) {
        printf("%02d:%02d until %s\n", hrs, mins, ps->status == CHARGING ? "full" : "depleted");
    }
    printf("%.2fW, ", (double) ps->powerNow / 1000000);
    printf("%.2fV, ", (double) ps->voltageNow / 1000000);
    printf("%.2fA\n", (double) ps->currentNow / 1000000);
    
}


int main() {
    //system("clear");
    FILE *fptr;
    fptr = fopen("/sys/class/power_supply/BAT0/uevent", "r");
    if (fptr == NULL) {
        printf("File not opened");
        exit(0);
    }
    
    struct PSData supplyData;
    printClock();
    psInit(&supplyData, fptr);
    printBattery(&supplyData);

    fclose(fptr);
    printf("\n");
    
    return 0;
}