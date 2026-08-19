#include "power.h"
#include "drivers/virtual/ramfb.h"
#include "lib/printf.h"
#include "drivers/virtual/fwcfg.h"
#include <stdint.h>

#include "gfx.h"

void kernel_main(void) {
    kprintf("Hello Computer\n");

    fw_config_init();
    uint16_t key; uint32_t size;
    if (fw_fg_find_file("etc/ramfb", &key, &size) == 0)
        kprintf("etc/ramfb: key %d size %d\n", key, size);
    else
        kprintf("etc/ramfb not found\n");

    ramfb_init();
    gfx_fill_rect(0, 0, 740, 480, 0x1010FF);
    for (;;);
}
