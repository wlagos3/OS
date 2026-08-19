//
// Created by William on 8/8/26.
//

#include <stdint.h>
#include "fwcfg.h"

#include "lib/printf.h"
#include "lib/string.h"


#define FW_CFG_BASE  0x09020000UL
#define FW_CFG_DATA (*(volatile uint8_t *)(FW_CFG_BASE))
#define FW_CFG_SELECTOR (*(volatile uint16_t *)(FW_CFG_BASE + 0x08)) // big endian
#define FW_CFG_DMA_ADDRESS (*(volatile uint64_t *)(FW_CFG_BASE + 0x10))

#define FW_CFG_FILE_DIR_SELECT 0x0019

typedef struct {
    uint32_t size;     /* big-endian */
    uint16_t select;   /* big-endian */
    uint16_t reserved;
    char     name[56];
} fwcfg_file;

typedef struct {
    uint32_t control;
    uint32_t length;
    uint64_t address;

} fwcfg_dma_access;

void fwcfg_dma_write(uint16_t key, void* data, uint32_t len) {
    volatile fwcfg_dma_access command;
    command.control = __builtin_bswap32(((uint32_t)key << 16) | 0x08 | 0x10);
    command.length = __builtin_bswap32(len);
    command.address = __builtin_bswap64((uint64_t)data);

    asm volatile("dsb sy" ::: "memory");
    FW_CFG_DMA_ADDRESS = __builtin_bswap64((uint64_t)&command);
    while (__builtin_bswap32(command.control) & ~0x01) {}
    if (__builtin_bswap32(command.control) & 0x01)
        kprintf("fwcfg: dma error\n");
}

void fwcfg_select(const uint16_t key) {
    FW_CFG_SELECTOR = __builtin_bswap16(key);
}

void fwcfg_read(void* buf, int len) {
    uint8_t *p = buf;
    while (len--) *p++ = FW_CFG_DATA;
}
char read_cfg_test() {
    return FW_CFG_DATA;
}

int fw_fg_find_file(const char* name, uint16_t* select, uint32_t* size) {
    fwcfg_select(FW_CFG_FILE_DIR_SELECT);
    uint8_t buf[4];
    fwcfg_read(&buf, 4);
    uint32_t count = buf[0] << 24 | buf[1] << 16 | buf[2] << 8 | buf[3];

    fwcfg_file f;
    for (uint32_t i = 0; i < count; i++) {
        fwcfg_read(&f, sizeof(f));
        if (streq(f.name, name)) {
            *select = __builtin_bswap16(f.select);
            *size = __builtin_bswap32(f.size);
            return 0;
        }
    }
    return -1;
}

void fw_config_init(void) {
    fwcfg_select(0);
    char sig[5] = {0};
    fwcfg_read(sig, 4);
    if (!streq(sig, "QEMU")) { kprintf("fwcfg: error\n"); return; }
    kprintf("fwcfg: ok\n");
}
