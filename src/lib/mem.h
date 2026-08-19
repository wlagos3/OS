//
// Created by William on 8/13/26.
//

#include <stddef.h>
#ifndef OS_MEM_H
#define OS_MEM_H

void *memcpy(void *dst, const void *src, size_t n);
void *memset(void *s, int c, size_t n);
void *memmove(void *dst, const void *src, size_t n);

#endif //OS_MEM_H
