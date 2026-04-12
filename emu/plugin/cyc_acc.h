#pragma once
#include "vector.h"
#include "cyc_model.h"

void ca_rt_init();
void ca_rt_fini();

void* ca_rt_add_insn(uint64_t pc, op_code_t opcode);
uint64_t* ca_rt_get_total_cycles_mem();
uint64_t ca_rt_get_total_cycles();
uint64_t ca_rt_get_total_instructions();

void ca_rt_insn_exec_cb(unsigned int vcpu, void* insn_index);

