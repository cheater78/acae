#!/usr/bin/python3 
import os
import subprocess
import select
import numpy as np
import pandas as pd

import matplotlib as mpl
import matplotlib.pyplot as plt
from matplotlib.ticker import FuncFormatter

import seaborn as sns
import seaborn.objects as so

from enum import Enum, auto
from typing import Literal

cwd: str = os.path.abspath(f"{__file__}/..")
eval_path: str = cwd
acae_path: str = os.path.abspath(f"{eval_path}/..")
results_path: str = os.path.abspath(f"{eval_path}/results")
plot_path: str = os.path.abspath(f"{eval_path}/plots")

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
                    if not silent:
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

def get_suite_benchmarks(suite: str) -> list[str]:
    suite_path: str = f"{acae_path}/benchmark/{suite}"
    cmd: str = f"make -C {suite_path} list"

    output = run_cmd(cmd, silent=True)

    suite_benchmarks: list[str] = []
    for line in output:
        if line.startswith("make:"):
            continue
        suite_benchmarks.append(line.strip())
    
    return suite_benchmarks

platforms: list[str] = [
    "acae",
    "native",
]

class Direction(Enum):
    MoreIsBetter = auto() # score, iterations/s
    LessIsBetter = auto() # time

suites: list[str] = [
    "embench"
]
suite_units: dict [str, str] = {
    "embench": "µs/iteration",
}
suite_direction: dict [str, Direction] = {
    "embench": Direction.LessIsBetter,
}

benchmarks: list[str] = [
    "dhrystone",
    "coremark",
]
benchmark_units: dict [str, str] = {
    "dhrystone": "DMIPS",
    "coremark": "CoreMark",
}
benchmark_direction: dict [str, Direction] = {
    "dhrystone": Direction.MoreIsBetter,
    "coremark": Direction.MoreIsBetter,
}

# add suite benchmarks by their tag
for suite in suites:
    for bench in get_suite_benchmarks(suite):
        benchmark_tag: str = f"{suite}-{bench}"
        benchmarks.append(benchmark_tag)
        benchmark_units[benchmark_tag] = suite_units[suite]
        benchmark_direction[benchmark_tag] = suite_direction[suite]

def capitalize(s: str, all: bool = False):
    if not s:
        return s
    if not all:
        return s[0].upper() + s[1:]
    caps: str = ""
    for c in s:
        caps += c.upper()
    return caps

def sns_bar_add_value_text(barplot) -> None:
    # text config
    fontsize=10
    color="black"
    fontweight='bold'
    precision=3
    margin_percent: tuple[float, float] = (0.00, 0.02)

    # plot characteristics
    x_min, x_max = barplot.get_xlim()
    y_min, y_max = barplot.get_ylim()
    margin: tuple[float, float] = (margin_percent[0] * (x_max - x_min), margin_percent[1] * (y_max - y_min))
    inv_transform = barplot.transData.inverted()

    # test fit text label horizontally to all bars
    labels_fit_horizontally: bool = True
    test_labels = []
    for bar in barplot.patches:
        bar_pos_x = bar.get_x()
        bar_width = bar.get_width()
        bar_height = bar.get_height()

        bar_value = bar_height

        test_label = barplot.text(
            bar_pos_x + bar_width / 2,
            bar_height,
            f'{bar_value:.{precision}f}',
            ha="center", va="bottom",
            fontsize=fontsize, fontweight=fontweight, color=color
        )
        test_labels.append(test_label)
        test_label_extent = test_label.get_window_extent()
        test_label_width = test_label_extent.width
        test_label_width_in_data = inv_transform.transform((test_label_width, 0))[0] - inv_transform.transform((0, 0))[0]
        
        if test_label_width_in_data > bar_width:
            labels_fit_horizontally = False
            for label in test_labels:
                label.remove()
            break
    if labels_fit_horizontally:
        return
    # -- labels did not fit horizontally
    
    for bar in barplot.patches:
        if not bar.get_visible():
            continue
        if bar.get_height() == 0:
            continue
        # real bar
        bar_pos_x = bar.get_x()
        bar_width = bar.get_width()
        bar_height = bar.get_height()

        bar_value = bar_height

        barplot.text(
            bar_pos_x + bar_width / 2,
            bar_height - margin[1],
            f'{bar_value:.{precision}f}',
            ha="center", va="top",
            fontsize=fontsize, color=color, fontweight=fontweight, rotation=-90)


######################################################################################################################

def plot_add_bar(dataset) -> plt.Axes:
    x_values = dataset["iterations"].unique()
    dataset_size = len(x_values)
    x_values_ld = np.log2(x_values).astype(int)

    x_ticks = np.arange(dataset_size)
    x_labels = [rf"$2^{{{x}}}$" for x in x_values_ld]

    avgd_dataset = dataset.groupby(["platform", "iterations"])["score"].mean().reset_index()

    score_bar_pl = sns.barplot(
        x=x_ticks,
        y="score",
        hue="platform",
        data=avgd_dataset,
        palette="Set2",
        legend=False
    )

    score_bar_pl.set_xticks(x_ticks)
    score_bar_pl.set_xticklabels(x_labels)

    return score_bar_pl

def plot_add_line(dataset) -> plt.Axes:
    x_values = dataset["iterations"].unique()
    dataset_size = len(x_values)
    x_values_ld = np.log2(x_values).astype(int)

    x_ticks = np.arange(dataset_size)
    x_labels = [rf"$2^{{{x}}}$" for x in x_values_ld]

    avgd_dataset = dataset.groupby(["platform", "iterations"])["score"].mean().reset_index()

    score_line_pl = sns.lineplot(
        x=x_ticks,
        y="score",
        hue="platform",
        data=avgd_dataset,
        palette="Set1",
        legend=False
    )

    score_line_pl.set_xticks(x_ticks)
    #score_line_pl.set_xticklabels(x_labels)

    return score_line_pl

def plot_add_box(dataset) -> plt.Axes:
    x_values = dataset["iterations"]
    x_values_unique = x_values.unique()
    dataset_size = len(x_values_unique)
    x_values_ld = np.log2(x_values).astype(int)

    x_ticks = np.arange(dataset_size)
    x_labels = [rf"$2^{{{x}}}$" for x in x_values_unique]

    score_box_pl = sns.boxplot(
        x=x_values_ld,
        y="score",
        hue="platform",
        data=dataset,
        palette="Set2",
        legend=False
    )

    #formatter = FuncFormatter(lambda x, _: f'{x:,.3f}')
    #score_box_pl.yaxis.set_major_formatter(formatter)

    score_box_pl.set_xticks(x_ticks)
    #score_box_pl.set_xticklabels(x_labels)

    return score_box_pl

def plot_benchmark(platform: str, benchmark: str, rotate_xlabel: bool = False) -> None:
    unit: str = benchmark_units[benchmark]
    plot_file: str = os.path.abspath(f"{plot_path}/{platform}/{platform}_{benchmark}_iter.pdf")
    parent_path: str = os.path.abspath(f"{plot_path}/{platform}")
    dataset_file: str = os.path.abspath(f"{results_path}/{platform}/{benchmark}.csv")
    dataset = pd.read_csv(dataset_file)

    plt.figure()
    score_bar_pl = plot_add_bar(dataset)
    sns_bar_add_value_text(score_bar_pl)
    #score_line_pl = plot_add_line(dataset)
    score_box_pl = plot_add_box(dataset)

    plt.xlabel("iterations")
    if rotate_xlabel:
        plt.xticks(rotation=45, ha="right")
    plt.ylabel("score")
    plt.title(f"{platform if platform != "acae" else "ACAE"}: {capitalize(benchmark)} score (in {unit})")

    os.makedirs(parent_path, exist_ok=True)
    plt.savefig(f"{plot_file}", bbox_inches="tight", format="pdf", transparent=True)

######################################################################################################################
def plot_benchmark_platform_comparison(benchmark: str) -> None:
    unit: str = benchmark_units[benchmark]
    plot_file: str = os.path.abspath(f"{plot_path}/{benchmark}_comp.pdf")
    parent_path: str = os.path.abspath(f"{plot_path}")
    
    reference_file: str = os.path.abspath(f"{eval_path}/reference/native/{benchmark}.csv")
    reference_available: bool = os.path.exists(reference_file)

    dataset_files: list[str] = \
        [ os.path.abspath(f"{results_path}/{platform}/{benchmark}.csv") for platform in platforms ] + \
        ([ reference_file ] if reference_available else [])
    datasets = [ pd.read_csv(dataset_file) for dataset_file in dataset_files ]

    dataset = pd.DataFrame()
    dataset["platform"] = [ (p if p != "acae" else "ACAE") for p in platforms ] + ([ "reference" ] if reference_available else [])
    
    # consider the avg. score of max iterations for each platform
    avgd_dataset = [ dataset[dataset['iterations'] == dataset['iterations'].max()]['score'].mean() for dataset in datasets ]

    plt.figure()
    score_bar_pl = sns.barplot(
        x="platform",
        y=avgd_dataset,
        hue="platform",
        data=dataset,
        legend=False
    )
    sns_bar_add_value_text(score_bar_pl)

    plt.xlabel("platform")
    plt.ylabel("score")
    plt.title(f"{capitalize(benchmark)} score (in {unit})")

    os.makedirs(parent_path, exist_ok=True)
    plt.savefig(f"{plot_file}", bbox_inches="tight", format="pdf", transparent=True)

######################################################################################################################

def plot_suite_benchmarks(platform: str, suite: str) -> None:
    suite_benchmarks: list[str] = get_suite_benchmarks(suite)

    for bench in suite_benchmarks:
        benchmark_tag: str = f"{suite}-{bench}"
        plot_benchmark(platform, benchmark_tag)

def plot_suite_platform_comparisons(suite: str) -> None:
    suite_benchmarks: list[str] = get_suite_benchmarks(suite)

    for bench in suite_benchmarks:
        benchmark_tag: str = f"{suite}-{bench}"
        plot_benchmark_platform_comparison(benchmark_tag)

######################################################################################################################

def plot_all_benchmark_deviation() -> None:
    plot_file: str = os.path.abspath(f"{plot_path}/deviations.pdf")
    parent_path: str = os.path.abspath(f"{plot_path}")

    dataset_files: list[str] = [
        os.path.abspath(f"{results_path}/{platform}/{benchmark}.csv") \
        for platform in platforms \
        for benchmark in benchmarks
    ]

    datasets: list[pd.DataFrame] = [pd.read_csv(dataset_file) for dataset_file in dataset_files]
    dataset = pd.concat(datasets, ignore_index=True)

    # optional - cleanup (shouldnt have anything else)
    dataset = dataset[dataset["platform"].isin(["native", "acae"])]

    # find maximum iteration with both platforms
    valid_iterations = (
        dataset.groupby(["benchmark", "iterations", "unit"])["platform"]
        .nunique()
        .reset_index(name="n_platforms")
    )

    # should never have another platform
    valid_iterations = valid_iterations[
        valid_iterations["n_platforms"] == 2
    ]

    # highest valid iteration per benchmark (+unit)
    max_iterations = (
        valid_iterations.groupby(["benchmark", "unit"])["iterations"]
        .max()
        .reset_index()
    )

    # Keep only those rows
    dataset = dataset.merge(
        max_iterations,
        on=["benchmark", "unit", "iterations"],
        how="inner",
    )

    # Put native and acae scores side-by-side
    comparison = dataset.pivot_table(
        index=["benchmark", "iterations", "unit"],
        columns="platform",
        values="score"
    ).reset_index()

    print(comparison[["benchmark", "iterations", "unit", "native", "acae"]])

    # Calculate relative deviations in both directions
    # correct for Direction = MoreIsBetter | LessIsBetter
    def performance_delta_percent(row: pd.Series, baseline: Literal["acae"] | Literal["native"]) -> float:
        measurement: str = ""
        if baseline == "acae":
            measurement = "native"
        else:
            measurement = "acae"

        if benchmark_direction[row["benchmark"]] == Direction.MoreIsBetter:
            # already MoreIsBetter -> just relative, convert to percent
            return (row[measurement] - row[baseline]) / row[baseline] * 100
        else:
            # LessIsBetter -> inverse relative, making it MoreIsBetter + convert to percent
            return (row[baseline] - row[measurement]) / row[baseline] * 100

    def performance_delta_percent_ref_to_native(row: pd.Series) -> float:
        return performance_delta_percent(row=row, baseline="native")
    def performance_delta_percent_ref_to_acae(row: pd.Series) -> float:
        return performance_delta_percent(row=row, baseline="acae")

    # direction corrected performance(MoreIsBetter) of native relative to acea
    comparison["native_deviation"] = comparison.apply(performance_delta_percent_ref_to_acae, axis=1)
    # direction corrected performance(MoreIsBetter) of acea relative to native
    comparison["acae_deviation"] = comparison.apply(performance_delta_percent_ref_to_native, axis=1)

    # Split the column -> for hue
    benchmark_map: dict[str, tuple[str, str]] = {}
    for suite in suites:
        for full_benchmark in benchmarks:
            if full_benchmark.startswith(f"{suite}-"):
                benchmark_name = full_benchmark[len(suite) + 1:]
                benchmark_map[full_benchmark] = (suite, benchmark_name)
    
    def split_suite_benchmark(full_benchmark: str) -> tuple[str, str]:
            # return the tuple mapped or just (suite=benchmark, benchmark)
            return benchmark_map.get(full_benchmark, (full_benchmark, full_benchmark))
    comparison[["suite", "benchmark"]] = pd.DataFrame(
        comparison["benchmark"].apply(split_suite_benchmark).tolist(),
        index=comparison.index,
    )

    # Final dataframe for plotting
    plot_dataframe = comparison[
        [
            "suite",
            "benchmark",
            "iterations",
            "unit",
            "native",
            "acae",
            "native_deviation",
            "acae_deviation",
        ]
    ]

    plt.figure()
    deviation_bar_plt = sns.barplot(
        x="benchmark",
        y="acae_deviation",
        hue="suite",
        data=plot_dataframe,
        # errorbar=None,
        legend=True
    )
    sns_bar_add_value_text(deviation_bar_plt)

    plt.xticks(rotation=45, ha="right")
    plt.subplots_adjust(
        bottom=0.3,  # more room for x labels
        top=0.9      # more room for title
    )

    plt.xlabel("benchmark")
    plt.ylabel("deviation")
    plt.title(f"Deviation of ACAE relative to native (in %)")

    os.makedirs(parent_path, exist_ok=True)
    plt.savefig(f"{plot_file}", format="pdf", transparent=True)

######################################################################################################################

# individual avg benchmark score per iterations bar plots
plot_benchmark("acae", "dhrystone")
plot_benchmark("native", "dhrystone")
plot_benchmark_platform_comparison("dhrystone")

plot_benchmark("acae", "coremark")
plot_benchmark("native", "coremark")
plot_benchmark_platform_comparison("coremark")

plot_suite_benchmarks("acae", "embench")
plot_suite_benchmarks("native", "embench")
plot_suite_platform_comparisons("embench")

plot_all_benchmark_deviation()

# plt.show()