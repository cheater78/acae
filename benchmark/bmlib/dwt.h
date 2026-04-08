#pragma once
#define DWT_CTRL    *(volatile unsigned int*) 0x40000000
#define DWT_CYCCNT  *(volatile unsigned int*) 0x40000004
#define DWT_LAR     *(volatile unsigned int*) 0x40000FB0
//#define SCB_DEMCR   *(volatile unsigned int*) 0xE000EDFC

void dwt_init();