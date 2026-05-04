#!/bin/bash
CWD="$( cd "$( dirname "${BASH_SOURCE[0]}" )" > /dev/null && pwd )"

ACAEPATH=$1
cd "${ACAEPATH}"

PLATFORM="acae"
BENCHMARK="coremark"

# Dhry
# 1,21,2,true
# CoreMark
# 855,21,140?,false -> 855 < i < 3587
iter_begin=1
iter_steps=11
iter_step=2
iter_exp=true

OUTPUTFILE="${CWD}/${PLATFORM}-${BENCHMARK}.csv"

save_score() {
    platform=$1
    benchmark=$2
    iterations=$3
    bench_output=$4

    if ! [[ -f "$OUTPUTFILE" ]]; then
        mkdir -p "$(dirname "$OUTPUTFILE")"
        echo "platform,iterations,score,score_per_MHz" > "$OUTPUTFILE"
    fi

    score=$(echo "${bench_output}" | grep -oP 'Score:\s*\K[0-9.]+')
    score_per_MHz=$(echo "${bench_output}" | grep -oP 'Score/MHz:\s*\K[0-9.]+')

    echo "${platform},${iterations},${score},${score_per_MHz}" >> "$OUTPUTFILE"
}

run_bench() {
    benchmark=$1
    iterations=$2

    build_output=$(ITERATIONS=$iterations make -C "${ACAEPATH}/benchmark/${benchmark}/" clean all 2>&1)
    #echo "${build_output}"
    run_output=$("${ACAEPATH}/emu/run.sh" "${ACAEPATH}/benchmark/${benchmark}/bin/${benchmark}.elf" 2>&1)
    #echo "${run_output}"

    # retrieve 
    save_score "${PLATFORM}" "${benchmark}" "${iterations}" "${run_output}"
}

run_bench_batch() {
    benchmark=$1

    echo "Running ${benchmark} ${iter_steps} times, starting with ${iter_begin} iterations."
    iterations_target=$iter_begin

    for ((i=0; i<iter_steps; i++)); do
        
        echo "Running ${benchmark} [${i}]: ${iterations_target} iterations."
        run_bench "${benchmark}" "${iterations_target}"

        # incr.
        if [ "$iter_exp" = true ]; then
            iterations_target=$((iterations_target * iter_step))
        else
            iterations_target=$((iterations_target + iter_step))
        fi
    done
}

if [[ -f "$OUTPUTFILE" ]]; then
    rm "$OUTPUTFILE"
fi

run_bench_batch $BENCHMARK