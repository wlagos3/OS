//
// Created by William on 8/9/26.
//

#include "printf.h"
#include "console.h"
#include <stdarg.h>

void print_num(int num, int base) {
    char buf[24];
    int curr = num;
    int i;
    for (i = 0; curr > 0; i++) {
        buf[i] = (curr % base) ;
        curr = curr / base;
    }
    for (int j = i - 1; j >= 0; j--) {
        if (buf[j] == 10) {
            console_putc('A');
        }
        else if (buf[j] == 11) {
            console_putc('B');
        }
        else if (buf[j] == 12) {
            console_putc('C');
        }
        else if (buf[j] == 13) {
            console_putc('D');
        }
        else if (buf[j] == 14) {
            console_putc('E');
        }
        else if (buf[j] == 15) {
            console_putc('F');
        }
        else {
            console_putc(buf[j] + '0');
        }
    }
}

void kprintf(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    for (; *fmt; fmt++) {
        if (*fmt == '%') {
            char next = *(fmt + 1);
            if (next == 'd') {
                int d = va_arg(args, int);
                print_num(d, 10);
                fmt++;
            }
            else if (next == 's') {
                console_puts(va_arg(args, char *));
                fmt++;
            }
            else if (next == 'p') {
                int p = va_arg(args, int);
                print_num(p, 16);
                fmt++;
            }
            else if (next == 'c') {
                char c = va_arg(args, int );
                console_putc(c);
                fmt++;
            }
        }
        else {
            console_putc(*fmt);
        }
    }
    va_end(args);
}
