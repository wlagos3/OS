//
// Created by William on 8/9/26.
//

#include "string.h"

int streq(const char *s1, const char *s2) {
    while (*s1 && *s1 == *s2 ) {s1++; s2++;}
    return (*s1 == *s2);
}
