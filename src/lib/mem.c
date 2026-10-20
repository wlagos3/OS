//
// Created by William on 8/13/26.
//

#include "mem.h"

#include <stdint.h>
#include "printf.h"


static unsigned long memcpy_calls, memcpy_bytes, memset_calls, memset_bytes, memmove_calls, memcmp_calls;

static void pnum(unsigned long v) {
    if (v == 0) kprintf("0"); else kprintf("%d", (int)v);
}

void mem_stats(void) {
    kprintf("memcpy calls="); pnum(memcpy_calls);
    kprintf(" bytes="); pnum(memcpy_bytes);
    kprintf(" | memset calls="); pnum(memset_calls);
    kprintf(" bytes="); pnum(memset_bytes);
    kprintf(" | memmove calls="); pnum(memmove_calls);
    kprintf(" | memcmp calls="); pnum(memcmp_calls);
    kprintf("\n");
}

void *memcpy(void *dst, const void *src, size_t n) {
    memcpy_calls++; memcpy_bytes += n;
    uint8_t *d = dst;
    const uint8_t *s = src;
    for (size_t i = 0; i < n; i++) *d++ = *s++;
    return dst;
}

void *memset(void *s, int c, size_t n) {
    memset_calls++; memset_bytes += n;
    uint8_t *d = s;
    for (size_t i = 0; i < n; i++)
        *d++ = c;
    return s;
}
void *memmove(void *dst, const void *src, size_t n) {
    memmove_calls++;
    if (dst < src)  return memcpy(dst, src, n);
    if (dst == src) return dst;
    uint8_t *d = dst;
    const uint8_t *s = src;
    for (size_t i = n; i --> 0;) d[i] = s[i];
    return dst;
}

int memcmp(const void *a, const void *b, size_t n) {
    memcmp_calls++;
    const uint8_t *d = a;
    const uint8_t *s = b;
    for (size_t i = 0; i < n; i++) {
        if (d[i] != s[i]) {
            return d[i] - s[i];
        }
    }
    return 0;
}
