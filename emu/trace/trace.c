#include <sched.h>
#include <stdint.h>
#include <stdio.h>
#include <glib-2.0/glib.h>
#include <plugins/qemu-plugin.h>
#include <stdlib.h>
#include <string.h>

#include "cycle_model.h"

QEMU_PLUGIN_EXPORT int qemu_plugin_version = QEMU_PLUGIN_VERSION;

#define F_CLK 100000000UL
// Cycle Counter Address -> custom DWT_CYCCNT
static const uint64_t cyccnt_addr = 0x40000004;

// Global cycle counter
static uint64_t total_cycles = 0;
static uint64_t total_instructions = 0;
static FILE *log_file = NULL;

static void memory_read_access_cb(
    unsigned int vcpu_index,
    qemu_plugin_meminfo_t info,
    uint64_t vaddr,
    void *userdata) {
    // handle reads from cyccnt_addr only
    if (vaddr != cyccnt_addr) {
        return;
    }
    GByteArray cyc_value = {
        (guint8*)&total_cycles,
        (guint)4
    };
    qemu_plugin_write_memory_vaddr(vaddr, &cyc_value);
}

typedef struct {
    op_code_t opcode;
    uint64_t pc;
    uint32_t cycles;
} insn_info;

struct {
    uint64_t size;
    insn_info* data;
} insns = {0, NULL};

insn_info* push_insn(const insn_info* insn) {
    // Resize array
    insn_info* new_data = realloc(insns.data, (insns.size + 1) * sizeof(insn_info));
    if (!new_data) return NULL; // allocation failed

    insns.data = new_data;
    insns.data[insns.size] = *insn; // copy the struct
    return &insns.data[insns.size++];
}

void free_insns() {
    free(insns.data);
    insns.data = NULL;
    insns.size = 0;
}

// Instruction callback
static void insn_exec_cb(unsigned int vcpu, void *userdata) {
    insn_info *insn = userdata;
    const uint32_t cycles = get_instruction_cycles(insn->opcode) + get_memory_access_cycles(insn->pc);
    total_cycles += cycles;
    total_instructions++;
}

void trace_trans_cb(qemu_plugin_id_t id, struct qemu_plugin_tb *tb) {
    size_t n = qemu_plugin_tb_n_insns(tb);
    for (size_t i = 0; i < n; i++) {
        struct qemu_plugin_insn *insn = qemu_plugin_tb_get_insn(tb, i);
        
        // register a memory access callback -> DWT_CYCCNT reads
        qemu_plugin_register_vcpu_mem_cb(
            insn,
            memory_read_access_cb,
            QEMU_PLUGIN_CB_RW_REGS,
            QEMU_PLUGIN_MEM_RW,
            NULL);

        op_code_t opcode = 0; // Thumb2 -> T32 is the largest enconding
        uint64_t pc = qemu_plugin_insn_vaddr(insn);
        const size_t opcode_size = qemu_plugin_insn_data(insn, &opcode, 4);

        insn_info iinfo;
        iinfo.opcode = opcode;
        iinfo.pc = pc;
        insn_info* info = push_insn(&iinfo);
        
        // register cpu instruction callback -> per instruction cycle accounting
        qemu_plugin_register_vcpu_insn_exec_cb(
            insn,
            insn_exec_cb,
            QEMU_PLUGIN_CB_NO_REGS, // not accessing any registers
            (void *)info);
    }
}

static void qemu_plugin_exit(qemu_plugin_id_t id, void* userdata) {
    const uint64_t runtime_us = (total_cycles * 1000000UL) / F_CLK;
    uint64_t mips = runtime_us ? (total_instructions * 1000000UL) / runtime_us : 0;
    fprintf(log_file,
        "Instructions: %lu \nCycles: %lu \nEqivalent Time(us): %lu \nMIPS: %lu",
        total_instructions, total_cycles, runtime_us, mips);
    if (log_file) {
        fclose(log_file);
        log_file = NULL;
    }
    free_insns();
}

// Plugin install
QEMU_PLUGIN_EXPORT int qemu_plugin_install(qemu_plugin_id_t id,
                                           const qemu_info_t *info,
                                           int argc, char **argv) {
    const char *filename = "cycle_trace.log";
    if (argc > 0 && argv[0]) {
        filename = argv[0];
    }

    log_file = fopen(filename, "w");
    if (!log_file) {
        return -1;
    }

    // register a translation cb -> whenever an op is translated to a native op
    qemu_plugin_register_vcpu_tb_trans_cb(
        id,
        trace_trans_cb
    );

    qemu_plugin_register_atexit_cb(id, qemu_plugin_exit, NULL);
    return 0;
}


