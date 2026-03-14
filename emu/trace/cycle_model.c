#include "cycle_model.h"

typedef struct {
    uint32_t start;
    uint32_t end;
    uint32_t waitstates;
} mem_region_t;

#define NUM_REGIONS 2
static mem_region_t memory_regions[NUM_REGIONS] = {
    {0x08000000, 0x08020000, 1}, // Flash: 1 waitstate
    {0x20000000, 0x20010000, 0}  // SRAM: 0 waitstate
};

// Simplified cycle model for demo (replace with real opcode table)
uint32_t get_instruction_cycles(op_code_t opcode) {
    // Thumb2 16-bit: assume 1 cycle per ALU, 2 cycles for load/store
    // Replace with detailed vendor table for accurate timing
    if ((opcode & 0xF800) == 0x6800) { // LDR reg, [reg]
        return 2;
    } else if ((opcode & 0xF800) == 0x6000) { // STR reg, [reg]
        return 2;
    } else if ((opcode & 0xF800) == 0xD000) { // Bcc branch
        return 1;
    }
    return 1; // default 1 cycle
}

// Estimate memory stall based on PC address
uint32_t get_memory_access_cycles(address_t addr) {
    for (int i = 0; i < NUM_REGIONS; i++) {
        if (addr >= memory_regions[i].start && addr < memory_regions[i].end) {
            return memory_regions[i].waitstates;
        }
    }
    return 0;
}