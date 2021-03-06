/**
 * Explicitly no license, feel free to do whatever the hell you want to this mess.
 **/

#ifndef COLOR_H
#define COLOR_H

#include <stdio.h>

enum color_set_t {
    SET_RESET = 0,
    SET_BOLD = 1,
    SET_DIM = 2,
    SET_UNDERLINED = 4,
    SET_BLINK = 5,
    SET_REVERSE = 7,
    SET_HIDDEN = 8,
    // I don't like the other set/resets

    FG_DEFAULT = 39,
    FG_BLACK = 30,
    FG_RED = 31,
    FG_GREEN = 32,
    FG_YELLOW = 33,
    FG_BLUE = 34,
    FG_MAGENTA = 35,
    FG_CYAN = 36,
    FG_LIGHT_GRAY = 37,
    FG_DARK_GRAY = 90,
    FG_LIGHT_RED = 91,
    FG_LIGHT_GREEN = 92,
    FG_LIGHT_YELLOW = 93,
    FG_LIGHT_BLUE = 94,
    FG_LIGHT_MAGENTA = 95,
    FG_LIGHT_CYAN = 96,
    FG_WHITE = 97,
    BG_DEFAULT = 49,
    BG_BLACK = 40,
    BG_RED = 41,
    BG_GREEN = 42,
    BG_YELLOW = 43,
    BG_BLUE = 44,
    BG_MAGENTA = 45,
    BG_CYAN = 46,
    BG_LIGHT_GRAY = 47,
    BG_DARK_GRAY = 100,
    BG_LIGHT_RED = 101,
    BG_LIGHT_GREEN = 102,
    BG_LIGHT_YELLOW = 103,
    BG_LIGHT_BLUE = 104,
    BG_LIGHT_MAGENTA = 105,
    BG_LIGHT_CYAN = 106,
    BG_WHITE = 107,
};

#define CLEAR_MODIFIERS() text_set(SET_RESET, FG_DEFAULT, BG_DEFAULT)

void text_set(int set, int fg, int bg);

#endif // COLOR_H