//
// Created by William on 8/8/26.
//
#include <stdint.h>

#ifndef OS_FWCFG_H
#define OS_FWCFG_H
void fw_config_init(void);
int fw_fg_find_file(const char* name, uint16_t* select, uint32_t* size);
void fwcfg_dma_write(uint16_t key, void* data, uint32_t len);


#endif //OS_FWCFG_H
