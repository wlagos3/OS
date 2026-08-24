//
// Created by William on 8/13/26.
//

#include "mem.h"

#include <stdint.h>
#include "printf.h"


void *memcpy(void *dst, const void *src, size_t n) {
    kprintf("calling memcpy\n");
    uint8_t *d = dst;
    const uint8_t *s = src;
    for (size_t i = 0; i < n; i++) *d++ = *s++;
    return dst;
}

void *memset(void *s, int c, size_t n) {
    kprintf("calling memset\n");
    uint8_t *d = s;
    for (size_t i = 0; i < n; i++)
        *d++ = c;
    return s;
}
void *memmove(void *dst, const void *src, size_t n) {
    kprintf("yo");
    if (dst < src)  return memcpy(dst, src, n);
    if (dst == src) return dst;
    uint8_t *d = dst;
    const uint8_t *s = src;
    for (size_t i = n; i --> 0;) d[i] = s[i];
    return dst;
}

int memcmp(const void *a, const void *b, size_t n) {
    kprintf("yo");
    const uint8_t *d = a;
    const uint8_t *s = b;
    for (size_t i = 0; i < n; i++) {
        if (d[i] != s[i]) {
            return d[i] - s[i];
        }
    }
    return 0;
}
