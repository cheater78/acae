#!/bin/python3
import os
import subprocess
import select
import shutil
import re

cwd: str = os.path.abspath(f"{__file__}/..")
eval_path: str = cwd
acae_path: str = os.path.abspath(f"{eval_path}/..")

def run_cmd(cmd: str, silent: bool = False) -> list[str]:
    with subprocess.Popen(
        cmd,
        shell=True,
        text=True,
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE) as process:
        output_lines: list[str] = []
        stdout_closed = False
        stderr_closed = False
        while True:
            fds = []
            if not stdout_closed:
                fds.append(process.stdout)
            if not stderr_closed:
                fds.append(process.stderr)
            if not fds:
                break
            rlist, _, _ = select.select(fds, [], [])
            
            for stream in rlist:
                line = stream.readline()
                if line:
                    print(line, end='')
                    output_lines.append(line)
                else:
                    if stream == process.stdout:
                        process.stdout.close()
                        stdout_closed = True
                    if stream == process.stderr:
                        process.stderr.close()
                        stderr_closed = True

            if stdout_closed and stderr_closed and process.poll() is not None:
                break

        return output_lines

def create_iterations(start: int = 1, steps: int = 10, step_pow2: bool = True, step_pow10: bool = False, step_lin_by: int = 0) -> list[int]:
    iterations: list[int] = []

    for istep in range(0, steps):
        if step_pow2:
            iterations.append(pow(2, start + istep))
        if step_pow10:
            iterations.append(pow(10, start + istep))
        if step_lin_by != 0:
            iterations.append((start + istep) * step_lin_by)
    
    iterations.sort()
    return iterations

def wipe_benchmark_results():
    results_path: str = os.path.abspath(f"{cwd}/results")
    if os.path.exists(results_path):
        shutil.rmtree(results_path)


def save_benchmark_result(platform: str, benchmark: str, iterations: int, score: float, score_per_mhz: float, unit: str):
    parent_path: str = os.path.abspath(f"{cwd}/results/{platform}")
    file_path: str = os.path.abspath(f"{parent_path}/{benchmark}.csv")
    header: str =  "platform,benchmark,iterations,score,score_per_mhz,unit"
    result: str = f"{platform},{benchmark},{iterations},{score},{score_per_mhz},{unit}"
    print(f"Saving Result: {result} to file: {file_path}")

    os.makedirs(parent_path, exist_ok=True)
    file_new: bool = not os.path.exists(file_path)
    with open(file_path, 'a') as file:
        if file_new:
            file.write(f"{header}\n")
        file.write(f"{result}\n")

def save_benchmark_output(platform: str, benchmark: str, iterations: int, log: str) -> None:
    parent_path: str = os.path.abspath(f"{cwd}/log/{platform}")
    file_path: str = os.path.abspath(f"{parent_path}/{benchmark}_{iterations}iterations.log")

    print(f"Saving Log to file: {file_path}")
    os.makedirs(parent_path, exist_ok=True)
    with open(file_path, 'w') as file:
        file.write(f"{log}")

def get_suite_benchmarks(suite: str) -> list[str]:
    suite_path: str = f"{acae_path}/benchmark/{suite}"
    cmd: str = f"make -C {suite_path} list"

    output = run_cmd(cmd)

    suite_benchmarks: list[str] = []
    for line in output:
        if line.startswith("make:"):
            continue
        suite_benchmarks.append(line.strip())
    
    return suite_benchmarks

def run_benchmark(platform: str, benchmark: str, iterations: int, suite_benchmark: str | None = None) -> None:
    
    suite_benchmarks:  list[str] = get_suite_benchmarks(benchmark)

    if len(suite_benchmarks) < 1:
        raise RuntimeError(f"Benchmark {benchmark} did not list its benchmarks! (Even non suite BMs need to report themselves)")

    is_suite: bool = len(suite_benchmarks) > 1
    benchmark_path: str = f"{acae_path}/benchmark/{benchmark}" # both bm/suite path
    
    if is_suite and (suite_benchmark is not None):
        if suite_benchmark not in suite_benchmarks:
            raise RuntimeError(f"Provided suite benchmark {suite_benchmark} is not part of {benchmark}!")
        suite_benchmarks = [ suite_benchmark ]

    for bench in suite_benchmarks:
        cmd: str = f"PLATFORM={platform} ITERATIONS={iterations} BENCHMARK={bench} make -C {benchmark_path} all"

        output = run_cmd(cmd)

        score: float = -1
        score_per_mhz: float = -1
        unit: str = ""
        for line in output:
            score_match = re.search(r"Score:\s*([0-9.]+)\s*(\S+)", line)
            score_per_mhz_match = re.search(r"Score/MHz:\s*([0-9.]+)\s*(\S+)", line)

            if score_match:
                if score != -1:
                    print("Score appeared twice!")
                    exit(1)
                score = float(score_match.group(1))
                unit = str(score_match.group(2))
                continue
            if score_per_mhz_match:
                if score_per_mhz != -1:
                    print("Score appeared twice!")
                    exit(1)
                score_per_mhz = float(score_per_mhz_match.group(1))
                unit = str(score_per_mhz_match.group(2))
                continue
        
        if score == -1 or score_per_mhz == -1 or unit == "":
            print("Score or Score/MHz or unit was not found!")
            exit(1)

        benchmark_tag: str = f"{benchmark}-{bench}" if is_suite else benchmark
        save_benchmark_result(platform, benchmark_tag, iterations, score, score_per_mhz, unit)
        save_benchmark_output(platform, benchmark_tag, iterations, "".join(output))

def run_benchmark_series(platform: str, benchmark: str, iterations: list[int]) -> None:
    for iteration_count in iterations:
        run_benchmark(platform, benchmark, iteration_count)
        
def run_benchmark_series_nondeterministic(platform: str, benchmark: str, iterations: list[int], samples: int = 3) -> None:
    for _ in range(0, samples):
        run_benchmark_series(platform, benchmark, iterations)


platforms: list[str] = [
    "native",
    "acae",
]
benchmarks: list[str] = [
    "dhrystone",
    "coremark",
    "embench",
]

# uncomment what you need
# NOTE: THIS WILL NOT RUN ONESHOT!

# WIPE ALL BENCHMARK RESULTS - ARE YOU SHURE?
#remove_this_safe_guard_if_YOU_REALLY_WANT_TO_DELETE_ALL_BENCHMARK_DATA wipe_benchmark_results() remove_this_safe_guard_if_YOU_REALLY_WANT_TO_DELETE_ALL_BENCHMARK_DATA

## DHRYSTONE
# Iterations = N[2⁰,2²⁰] (MIN: iter = 2⁰ (ALLOW_INVALID_RUNS), MAX: iter > ~2²⁰ -> cyccnt OF)
iterations = create_iterations(start = 0, steps = 21, step_pow2 = True)
#run_benchmark_series("native","dhrystone",iterations)
#run_benchmark_series_nondeterministic("native","dhrystone",iterations)
#run_benchmark_series("acae","dhrystone",iterations)
#run_benchmark_series_nondeterministic("acae","dhrystone",iterations)

## COREMARK
# Iterations = N[2⁰,2¹¹] (MIN: iter = 2⁰ (ALLOW_INVALID_RUNS), MAX: iter > ~3500 > 2¹¹ -> cyccnt OF)
iterations = create_iterations(start = 0, steps = 12, step_pow2 = True)
#run_benchmark_series("native","coremark",iterations)
#run_benchmark_series_nondeterministic("native","coremark",iterations)
#run_benchmark_series("acae","coremark",iterations)
#run_benchmark_series_nondeterministic("acae","coremark",iterations)

iterations = create_iterations(start = 0, steps = 12, step_pow2 = True)
#run_benchmark_series("native", "embench", iterations)
run_benchmark_series("acae", "embench", iterations)

## DEBUG / TEST
#run_benchmark("acae","dhrystone",1)
#run_benchmark("native","dhrystone",1)