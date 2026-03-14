#!/bin/bash
EMU_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

TASK_BIN=$1
ARGS=$2

"${EMU_DIR}/run.sh" \
    $TASK_BIN \
	"-S -gdb tcp::1234 ${ARGS}"
