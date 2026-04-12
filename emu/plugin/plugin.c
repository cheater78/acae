#include "plugins/qemu-plugin.h"
#include <glib-2.0/glib.h>

#include "cyc_acc.h"
#include "log.h"
#include "platform.h"

QEMU_PLUGIN_EXPORT int qemu_plugin_version = QEMU_PLUGIN_VERSION;

static void cb_insn_exec(unsigned int vcpu, void* insn_index) {
    ca_rt_insn_exec_cb(vcpu, insn_index);

    // TODO: the only performance improvement: prefilter mem access (from manual insn decode) at tb trans
    GByteArray cyc_value = {
        (guint8*)ca_rt_get_total_cycles_mem(),
        (guint)4U
    };
    qemu_plugin_write_memory_vaddr(DWT_ADDR_CYCCNT, &cyc_value);
    pllog("cyc_cnt read[AT %#lx]: cycles=%lu \n", DWT_ADDR_CYCCNT, ca_rt_get_total_cycles());
}

static void cb_tb_trans(qemu_plugin_id_t id, struct qemu_plugin_tb* tb) {
    const size_t insn_count = qemu_plugin_tb_n_insns(tb);
    for (size_t i = 0; i < insn_count; i++) {
        struct qemu_plugin_insn* insn = qemu_plugin_tb_get_insn(tb, i);
        const uint64_t pc = qemu_plugin_insn_vaddr(insn);
        op_code_t opcode = 0;
        const size_t opcode_size = qemu_plugin_insn_data(insn, &opcode, 4);

        void* userdata = ca_rt_add_insn(pc, opcode);
        
        qemu_plugin_register_vcpu_insn_exec_cb(
            insn,
            cb_insn_exec,
            QEMU_PLUGIN_CB_NO_REGS,
            userdata);
    }
}

static void cb_plugin_init(qemu_plugin_id_t id,
                           const qemu_info_t *info,
                           int argc, char **argv) {
    pllog_init("ca_plugin.log", true);
    ca_rt_init();
    /*
        Translation block translation callback
        as instructions are read and translated by QEMU,
        this is done in translation blocks (TB) which are continuous sequences of instructions (that end e.g. with a control flow change),
        whenever a TB is translated this cb is fired,
        from there other cbs can be registered
    */
    qemu_plugin_register_vcpu_tb_trans_cb(
        id,
        cb_tb_trans
    );
    pllog("CA Plugin started.\n");
}

static void cb_plugin_fini(qemu_plugin_id_t id, void* userdata) {
    ca_rt_fini();
    pllog("CA Plugin finished.\n");
    pllog_fini();
}

/*
    Plugin install
    called by QEMU when the plugin is loaded
*/
QEMU_PLUGIN_EXPORT int qemu_plugin_install(qemu_plugin_id_t id,
                                           const qemu_info_t *info,
                                           int argc, char **argv) {
    // plugin init (indirection for symmetry)
    cb_plugin_init(id, info, argc, argv);
    // plugin fini, natively as cb
    qemu_plugin_register_atexit_cb(id, cb_plugin_fini, NULL);
    return 0;
}