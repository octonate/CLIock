#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "clock.c"

#define BUF_SIZE 128
#define BAT_GRAPHIC_SIZE 256

char *clrs[] = { "\033[0m", "\033[30m", "\033[31m", "\033[32m", "\033[33m", "\033[34m", "\033[35m", "\033[36m", "\033[37m", "\033[90m", "\033[91m", "\033[92m", "\033[93m", "\033[94m", "\033[95m", "\033[96m", "\033[97m" };
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

char *genBatGraphic(struct PSData *ps) {
    char *batGraphic = malloc(BAT_GRAPHIC_SIZE * sizeof(char));
    char *batColor;
    if (ps->capacity > 60) {
        batColor = clrs[G];
    } else if (ps->capacity > 25) {
        batColor = clrs[Y];
    } else {
        batColor = clrs[R];
    }

    int width = round((double) ps->capacity / 5);

    snprintf(batGraphic, BAT_GRAPHIC_SIZE, "%sbat:%s\n", clrs[C], clrs[Z]);
    snprintf(batGraphic + strlen(batGraphic), BAT_GRAPHIC_SIZE, "[%s", batColor);

    for (int i = 0; i < width; i++) {
        snprintf(batGraphic + strlen(batGraphic), BAT_GRAPHIC_SIZE, "|");
    }
    snprintf(batGraphic + strlen(batGraphic), BAT_GRAPHIC_SIZE, "%s", clrs[Z]);
    for (int i = 0; i < 20 - width; i++) {
        snprintf(batGraphic + strlen(batGraphic), BAT_GRAPHIC_SIZE, "-");
    }
    snprintf(batGraphic + strlen(batGraphic), BAT_GRAPHIC_SIZE, "]\n");

    double timeLeft;
    if (ps->status == CHARGING) {
        timeLeft = (double) (ps->energyFull - ps->energyNow) / ps->powerNow;
    } else if (ps->status == DISCHARGING) {
        timeLeft = (double) ps->energyNow / ps->powerNow;
    }
    int hrs = floor(timeLeft);
    int mins = 60 * (timeLeft - floor(timeLeft));


    snprintf(batGraphic + strlen(batGraphic), BAT_GRAPHIC_SIZE, "%s", clrs[C]);
    snprintf(batGraphic + strlen(batGraphic), BAT_GRAPHIC_SIZE, "%.1f%% %s(%s)\n", ps->capacity, clrs[BK], statuses[ps->status]);
    if (ps->status != FULL) {
        snprintf(batGraphic + strlen(batGraphic), BAT_GRAPHIC_SIZE, "%02d:%02d until %s\n", hrs, mins, ps->status == CHARGING ? "full" : "depleted");
    }
    snprintf(batGraphic + strlen(batGraphic), BAT_GRAPHIC_SIZE, "%.2fW, ", (double) ps->powerNow / 1000000);
    snprintf(batGraphic + strlen(batGraphic), BAT_GRAPHIC_SIZE, "%.2fV, ", (double) ps->voltageNow / 1000000);
    snprintf(batGraphic + strlen(batGraphic), BAT_GRAPHIC_SIZE, "%.2fA\n", (double) ps->currentNow / 1000000);
    
    return batGraphic;
}


int main() {
    //system("clear");
    FILE *fptr;
    fptr = fopen("/sys/class/power_supply/BAT0/uevent", "r");
    if (fptr == NULL) {
        printf("File not opened");
        exit(0);
    }

    time_t t = time(NULL);
    struct tm curTime = *localtime(&t);
    char *clockGraphic = genClockGraphic(curTime.tm_hour, curTime.tm_min, curTime.tm_sec);
    printf("%s\n", clockGraphic);

    struct PSData supplyData;
    psInit(&supplyData, fptr);
    char *batGraphic = genBatGraphic(&supplyData);
    printf("%s", batGraphic);

    free(clockGraphic);
    free(batGraphic);
    fclose(fptr);
    printf("\n");
    
    return 0;
}
