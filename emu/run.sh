#!/bin/bash
EMU_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
EMU_CONFIG="${EMU_DIR}/config"
source "${EMU_CONFIG}"

QEMU="${EMU_DIR}/qemu"
QEMU_BUILD="${QEMU}/build"
QEMU_ARM="${QEMU_BUILD}/qemu-system-arm"
QEMU_PL_TRACE="${EMU_DIR}/trace/bin/trace.so"

TASK_BIN=$1
ARGS=$2

QEMU_TASK="$QEMU_ARM \
	-M $MODEL \
	-cpu $CPU \
	-nographic \
	-kernel $TASK_BIN \
	-semihosting \
	$ARGS \
	-icount shift=0,align=off,sleep=off" # \
	#-plugin $QEMU_PL_TRACE"

echo "${QEMU_TASK}"
taskset -c 0 $QEMU_TASK
	
