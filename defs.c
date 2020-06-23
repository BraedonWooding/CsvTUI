#include <termios.h>
#include <unistd.h>
#include <stdio.h>

#include "defs.h"

int getch(void) {
    // You can mostly ignore this
    // basically just getchar but won't echo
    struct termios oldattr, newattr;
    int ch;
    tcgetattr( STDIN_FILENO, &oldattr );
    newattr = oldattr;
    newattr.c_lflag &= ~( ICANON | ECHO );
    tcsetattr( STDIN_FILENO, TCSANOW, &newattr );
    ch = getchar();
    tcsetattr( STDIN_FILENO, TCSANOW, &oldattr );
    return ch;
}
