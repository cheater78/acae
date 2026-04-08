#!/bin/bash
EMU_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
EMU_CONFIG="${EMU_DIR}/config"
source "${EMU_CONFIG}"

QEMU="${EMU_DIR}/qemu"
QEMU_CONFIGURE="${QEMU}/configure"
QEMU_BUILD="${QEMU}/build"
QEMU_ARM="${QEMU_BUILD}/qemu-system-arm"
QEMU_PL_TRACE="${EMU_DIR}/trace"

echo "Building Qemu for ARM..."
cd $QEMU
chmod +x $QEMU_CONFIGURE
$QEMU_CONFIGURE --target-list=arm-softmmu --enable-plugins
ninja -C $QEMU_BUILD

# list supported ARM $CPU boards
chmod +x $QEMU_ARM
$QEMU_ARM -M help | grep $CPU

echo "Building Qemu cycle tracer..."
cd $QEMU_PL_TRACE
# build + compile_commands
bear -- make -C "${QEMU_PL_TRACE}" clean all