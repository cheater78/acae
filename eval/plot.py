#!/usr/bin/python3 
import os
import numpy as np
import pandas as pd

import matplotlib as mpl
import matplotlib.pyplot as plt
from matplotlib.ticker import FuncFormatter

import seaborn as sns
import seaborn.objects as so

cwd: str = os.path.abspath(f"{__file__}/..")
eval_path: str = cwd
acae_path: str = os.path.abspath(f"{eval_path}/..")
results_path: str = os.path.abspath(f"{eval_path}/results")
plot_path: str = os.path.abspath(f"{eval_path}/plots")

platforms: list[str] = [
    "acae",
    "native",
]

benchmarks: list[str] = [
    "dhrystone",
    "coremark",
]

benchmark_units: dict [str, str] = {
    "dhrystone": "DMIPS",
    "coremark": "CoreMark",
}

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
def plot_benchmark(platform: str, benchmark: str) -> None:
    unit: str = benchmark_units[benchmark]
    plot_file: str = os.path.abspath(f"{plot_path}/{platform}/{benchmark}.svg")
    parent_path: str = os.path.abspath(f"{plot_path}/{platform}")
    dataset_file: str = os.path.abspath(f"{results_path}/{platform}/{benchmark}.csv")
    dataset = pd.read_csv(dataset_file)

    x_values = dataset["iterations"].unique()
    dataset_size = len(x_values)
    x_values_ld = np.log2(x_values).astype(int)

    x_ticks = np.arange(dataset_size)
    x_labels = [rf"$2^{{{x}}}$" for x in x_values_ld]

    avgd_dataset = dataset.groupby(["platform", "iterations"])["score"].mean().reset_index()

    plt.figure()
    score_bar_pl = sns.barplot(
        x=x_ticks,
        y="score",
        hue="platform",
        data=avgd_dataset,
        palette="Set2",
        legend=False
    )
    sns_bar_add_value_text(score_bar_pl)
    score_line_pl = sns.lineplot(
        x=x_ticks,
        y="score",
        hue="platform",
        data=avgd_dataset,
        palette="Set1",
        legend=False
    )

    score_bar_pl.set_xticks(x_ticks)
    score_bar_pl.set_xticklabels(x_labels)

    plt.xlabel("iterations")
    plt.ylabel("score")
    plt.title(f"{platform}: {benchmark} score (in {unit})")

    os.makedirs(parent_path, exist_ok=True)
    plt.savefig(f"{plot_file}", bbox_inches="tight", format="svg", transparent=True)

######################################################################################################################
def plot_benchmark_platform_comparison(benchmark: str) -> None:
    unit: str = benchmark_units[benchmark]
    plot_file: str = os.path.abspath(f"{plot_path}/{benchmark}.svg")
    parent_path: str = os.path.abspath(f"{plot_path}")
    
    dataset_files: list[str] = \
        [ os.path.abspath(f"{results_path}/{platform}/{benchmark}.csv") for platform in platforms ] + \
        [ os.path.abspath(f"{eval_path}/reference/native/{benchmark}.csv") ]
    datasets = [ pd.read_csv(dataset_file) for dataset_file in dataset_files ]

    dataset = pd.DataFrame()
    dataset["platform"] = platforms + [ "reference" ]

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
    plt.title(f"{benchmark} score (in {unit})")

    os.makedirs(parent_path, exist_ok=True)
    plt.savefig(f"{plot_file}", bbox_inches="tight", format="svg", transparent=True)

######################################################################################################################
def plot_benchmark_distribution(platform: str, benchmark: str) -> None:
    unit: str = benchmark_units[benchmark]
    plot_file: str = os.path.abspath(f"{plot_path}/{platform}/{benchmark}-dist.svg")
    parent_path: str = os.path.abspath(f"{plot_path}/{platform}")
    dataset_file: str = os.path.abspath(f"{results_path}/{platform}/{benchmark}.csv")
    dataset = pd.read_csv(dataset_file)

    x_values = dataset["iterations"].unique()
    dataset_size = len(x_values)
    x_values_ld = np.log2(x_values).astype(int)

    x_ticks = np.arange(dataset_size)
    x_labels = [rf"$2^{{{x}}}$" for x in x_values_ld]

    plt.figure()
    score_box_pl = sns.boxplot(
        x="iterations",
        y="score",
        hue="platform",
        data=dataset,
        palette="Set2",
        legend=False
    )

    formatter = FuncFormatter(lambda x, _: f'{x:,.3f}')
    score_box_pl.yaxis.set_major_formatter(formatter)

    score_box_pl.set_xticks(x_ticks)
    score_box_pl.set_xticklabels(x_labels)

    plt.xlabel("iterations")
    plt.ylabel("score")
    plt.title(f"{platform}: {benchmark} score (in {unit})")

    os.makedirs(parent_path, exist_ok=True)
    plt.savefig(f"{plot_file}", bbox_inches="tight", format="svg", transparent=True)

######################################################################################################################

# individual avg benchmark score per iterations bar plots
plot_benchmark("acae", "dhrystone")
plot_benchmark("acae", "coremark")
plot_benchmark("native", "dhrystone")
plot_benchmark("native", "coremark")

# Platform comparison per benchmark bar plots
plot_benchmark_platform_comparison("dhrystone")
plot_benchmark_platform_comparison("coremark")

# individual benchmark score distribution per iterations box plots
plot_benchmark_distribution("acae", "dhrystone")
plot_benchmark_distribution("acae", "coremark")
plot_benchmark_distribution("native", "dhrystone")
plot_benchmark_distribution("native", "coremark")

plt.show()