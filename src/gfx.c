//
// Created by William on 8/13/26.
//

#include "gfx.h"
#include "drivers/virtual/ramfb.h"


void gfx_fill_rect(int x, int y, int w, int h, uint32_t color) {
    for (int row = y; row < y + h; row++) {
        for (int col = x; col < x + w; col++) {
            ramfb_fb[row][col] = color;
        }
    }
}
