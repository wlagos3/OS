//
// Created by William on 8/8/26.
//
#include <stdint.h>

#ifndef OS_RAMFB_H
#define OS_RAMFB_H


#define RAMFB_WIDTH  1920
#define RAMFB_HEIGHT 1080

extern uint32_t ramfb_fb[RAMFB_HEIGHT][RAMFB_WIDTH];

void ramfb_init(void);

#endif //OS_RAMFB_H
