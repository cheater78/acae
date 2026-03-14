#!/bin/bash
EMU_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

TASK_BIN=$1

arm-none-eabi-gdb ${TASK_BIN} -ex "target remote :1234"