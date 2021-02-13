#include "console.h"
#include <inttypes.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <stdio.h>

void console_set_cursor_pos(uint16_t x, uint16_t y) {
    printf("\033[%d;%dH", y, x);
}

void console_hide_cursor() {
    printf("\e[?25l");
}

void console_show_cursor() {
    printf("\e[?25h");
}

consize_t console_get_size() {
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

    consize_t consize;
    consize.w = w.ws_col;
    consize.h = w.ws_row;

    return consize;
}