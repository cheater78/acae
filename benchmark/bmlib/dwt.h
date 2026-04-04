#pragma once
#define DWT_CTRL *(volatile unsigned int*) 0xE0001000
#define DWT_CYCCNT *(volatile unsigned int*) 0xE0001004
#define DWT_LAR *(volatile unsigned int*) 0xE0001FB0
#define SCB_DEMCR *(volatile unsigned int*) 0xE000EDFC

void dwt_init();
unsigned int dwt_cyccnt();