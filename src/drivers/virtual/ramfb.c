//
// Created by William on 8/8/26.
//

#include <stdint.h>
#include "ramfb.h"

#include "lib/printf.h"
#include "fwcfg.h"


typedef struct __attribute__((packed)) {
    uint64_t addr;      /* be64: physical address of pixel array   */
    uint32_t fourcc;    /* be32: pixel format code                      */
    uint32_t flags;     /* be32: 0                                      */
    uint32_t width;     /* be32 */
    uint32_t height;    /* be32 */
    uint32_t stride;    /* be32: bytes per row = width * 4              */
} ramfb_config;


uint32_t ramfb_fb[1080][1920];

void ramfb_init(void) {
    uint16_t key; uint32_t size;
    if (fw_fg_find_file("etc/ramfb", &key, &size)) {
        kprintf("ramfb: not found\n");
        return;
    }

    ramfb_config cfg = {
        .addr   = __builtin_bswap64((uint64_t)ramfb_fb),
        .fourcc = __builtin_bswap32(0x34325258), /* XRGB8888 */
        .flags  = 0,
        .width  = __builtin_bswap32(1920),
        .height = __builtin_bswap32(1080),
        .stride = __builtin_bswap32(1920 * 4),
    };
    fwcfg_dma_write(key, &cfg, sizeof cfg);
}