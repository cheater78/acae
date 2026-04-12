#pragma once
#include <stdint.h>

typedef uint32_t op_code_t;
typedef uint32_t address_t;

uint32_t get_instruction_cycles(op_code_t op_code);
uint32_t get_memory_access_cycles(address_t address);