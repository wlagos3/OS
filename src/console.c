//
// Created by William on 8/8/26.
//

#include "console.h"

#include "drivers/pl011.h"

static int console_initialized = 0;

void console_init(void) {
    if (console_initialized) {
        return;
    }
    pl011_init();
    console_initialized = 1;
}

void console_putc(char c) {
    pl011_putc(c);
}
void console_puts(const char *s) {
    pl011_puts(s);
}