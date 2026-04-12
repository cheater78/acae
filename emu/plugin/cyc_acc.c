#include "cyc_acc.h"
#include "cyc_model.h"
#include "log.h"

typedef struct ca_insn_t {
    uint64_t pc;
    op_code_t opcode;
    uint64_t cycles;
    // here could be a cb func to inspect the instrcution arguments to evaluate the cycle cost more precisely
} ca_insn_t;

static vector_storage_t instructions;
uint64_t total_cycles;
uint64_t total_instructions;

void ca_rt_init() {
    instructions = vector_new(sizeof(ca_insn_t), 0);
    total_cycles = 0;
    total_instructions = 0;
}

void ca_rt_fini() {
    total_cycles = 0;
    total_instructions = 0;
    vector_delete(&instructions);
}

void* ca_rt_add_insn(uint64_t pc, op_code_t opcode) {
    const uint64_t insn_index = vector_size(&instructions);
    const bool v = vector_push(&instructions,
        &(ca_insn_t) {
            .pc = pc,
            .opcode = opcode,
            .cycles = 1 // TODO: cyc_model, 1 for now
        }
    );
    if (!v) {
        pllog("Failed to add instruction: pc=%#lx, opcode=%#x\n", pc, opcode);
        return NULL;
    }
    // pllog("Adding instruction[%zu]: pc=%#lx, opcode=%#x\n", insn_index, pc, opcode);
    return (void*)(uintptr_t)(insn_index + 1); // keep 0 as invalid index
}

uint64_t* ca_rt_get_total_cycles_mem(){
    return &total_cycles;
}

uint64_t ca_rt_get_total_cycles() {
    return total_cycles;
}
uint64_t ca_rt_get_total_instructions() {
    return total_instructions;
}

void ca_rt_insn_exec_cb(unsigned int vcpu, void* insn_index) {
    if (!insn_index) {
        pllog("Invalid instruction index: NULL\n");
        return;
    }
    const uint64_t vec_size = vector_size(&instructions);
    const uint64_t idx = ((uint64_t)(uintptr_t)insn_index - 1);
    if (vec_size <= idx){
        pllog("Invalid instruction index: %zu/%zu\n", idx, vec_size);
        return;
    }
    ca_insn_t* insn = (ca_insn_t*)vector_at(&instructions, idx);
    total_cycles += insn->cycles;
    total_instructions++;

    // pllog("Executed instruction at pc=%#lx, opcode=%#x, cycles=%zu\n", insn->pc, insn->opcode, insn->cycles);
}