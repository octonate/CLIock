#ifndef COLORS_H
#define COLORS_H

enum Color {
    DEFAULT,
    BLACK,
    RED,
    GREEN,
    YELLOW,
    BLUE,
    MAGENTA,
    CYAN,
    WHITE,
    BRIGHT_BLACK,
    BRIGHT_RED,
    BRIGHT_GREEN,
    BRIGHT_YELLOW,
    BRIGHT_BLUE,
    BRIGHT_MAGENTA,
    BRIGHT_CYAN,
    BRIGHT_WHITE
};

/* CHANGE THESE TO WHATEVER COLORS YOU WANT, AVAILABLE COLORS ARE SHOWN ABOVE */
const enum Color highlightTextColor = CYAN; 
const enum Color regularTextColor = BRIGHT_BLACK; 

const enum Color clockHourHandColor = CYAN;
const enum Color clockMinHandColor = GREEN;
const enum Color clockCenterColor = BRIGHT_BLACK;
const enum Color clockFrameColor = BLACK;
const enum Color clockTickMarkColor = BRIGHT_BLACK;
/*----------------------------------------------------------------------------*/

const char *ansiClrs[] = {
    "\033[0m",
    "\033[30m",
    "\033[31m",
    "\033[32m",
    "\033[33m",
    "\033[34m",
    "\033[35m",
    "\033[36m",
    "\033[37m",
    "\033[90m",
    "\033[91m",
    "\033[92m",
    "\033[93m",
    "\033[94m",
    "\033[95m",
    "\033[96m",
    "\033[97m",
};

#endif //COLORS_H
