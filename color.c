#include "color.h"

void text_set(int set, int fg, int bg) {
    printf("\e[%d;%d;%dm", set, fg, bg);
}