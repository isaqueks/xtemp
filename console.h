#ifndef CONSOLE_H
#define CONSOLE_H

#include <inttypes.h>
#include <sys/ioctl.h>
#include <unistd.h>

typedef struct {
    uint16_t w;
    uint16_t h;
} consize_t;

void console_set_cursor_pos(uint16_t x, uint16_t y);
void console_hide_cursor();
void console_show_cursor();
consize_t console_get_size();

#endif