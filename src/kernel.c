#include "power.h"
#include "drivers/virtual/ramfb.h"
#include "lib/printf.h"
#include "drivers/virtual/fwcfg.h"
#include <stdint.h>


#include "gfx.h"

extern const int32_t bar;
extern uint32_t add(uint32_t);

void kernel_main(void) {
    kprintf("Hello Computer\n");
    kprintf("bar from hello.k1 = %d\n", bar);

    fw_config_init();
    uint16_t key; uint32_t size;
    if (fw_fg_find_file("etc/ramfb", &key, &size) == 0)
        kprintf("etc/ramfb: key %d size %d\n", key, size);
    else
        kprintf("etc/ramfb not found\n");

    ramfb_init();
    gfx_fill_rect(0, 0, 740, 480, 0x1010FF);

    uint32_t ten = 10;
    uint32_t new_num = add(ten);
    kprintf("num: %d\n", new_num);

    for (;;);
}
