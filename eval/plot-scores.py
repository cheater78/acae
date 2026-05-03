#!/usr/bin/python3 
import numpy as np
import pandas as pd

import matplotlib as mpl
import matplotlib.pyplot as plt

import seaborn as sns
import seaborn.objects as so

######################################################################################################################
def plot_acae_bench(benchmark: str, unit: str, ld_scale: bool):
    dataset = pd.read_csv("acae-" + benchmark + ".csv")
    plt.figure()
    
    x_dataset_name = "iterations_ld" if ld_scale else "iterations"
    if ld_scale:
        dataset["iterations_ld"] = np.log2(dataset["iterations"]).astype(int)
    
    score_bar_pl = sns.barplot(
        x=x_dataset_name,
        y="score",
        hue="platform",
        data=dataset,
        palette="Set2",
        legend=False
    )
    score_line_pl = sns.lineplot(
        x=x_dataset_name,
        y="score",
        hue="platform",
        data=dataset,
        palette="Set1",
        legend=False
    )

    score_xticks = dataset[x_dataset_name].unique()
    score_xlabels = [rf"$2^{{{x}}}$" for x in score_xticks] if ld_scale else score_xticks

    score_bar_pl.set_xticks(score_xticks)
    score_bar_pl.set_xticklabels(score_xlabels)

    plt.xlabel("iterations")
    plt.ylabel("score")
    plt.title(f"{benchmark} score (in {unit})")
    plt.savefig(f"pl-{benchmark}-acae-score-over-iter.svg", bbox_inches="tight", format="svg", transparent=True)

plot_acae_bench("dhrystone", "DMIPS", True)

######################################################################################################################
def plot_acae_bench_(benchmark: str, unit: str, ld_scale: bool):
    dataset = pd.read_csv("acae-" + benchmark + ".csv")
    plt.figure()

    score_xlabels = dataset["iterations"].unique()
    score_xticks = np.arange(len(score_xlabels))

    score_bar_pl = sns.barplot(
        x=score_xticks,
        y="score",
        hue="platform",
        data=dataset,
        palette="Set2",
        legend=False
    )
    score_line_pl = sns.lineplot(
        x=score_xticks,
        y="score",
        hue="platform",
        data=dataset,
        palette="Set1",
        legend=False
    )

    score_bar_pl.set_xticks(score_xticks)
    score_bar_pl.set_xticklabels(score_xlabels)
    plt.xticks(rotation=45)
    plt.xlabel("iterations")
    plt.ylabel("score")
    plt.title(f"{benchmark} score (in {unit})")
    plt.savefig(f"pl-{benchmark}-acae-score-over-iter.svg", bbox_inches="tight", format="svg", transparent=True)

plot_acae_bench_("coremark", "CoreMark", False)

######################################################################################################################
def plot_acae_v_ref_bench(benchmark: str, unit: str):
    dataset_acae = pd.read_csv("acae-" + benchmark + ".csv")
    dataset_ref = pd.read_csv("ref-" + benchmark + ".csv")
    plt.figure()
    ref_score = dataset_ref["score"].iloc[-1]
    acae_score = dataset_acae["score"].iloc[-1] # most iterations value

    dataset_ref_v_acae = pd.DataFrame()
    dataset_ref_v_acae["platform"] = ["acae", "reference"]
    dataset_ref_v_acae["score"] = [acae_score, ref_score]

    score_ref_v_acae = sns.barplot(
        x="platform",
        y="score",
        hue="platform",
        data=dataset_ref_v_acae,
        legend=False
    )
    plt.xlabel("platform")
    plt.ylabel("score")
    plt.title(f"{benchmark} score (in {unit})")
    plt.savefig(f"pl-{benchmark}-acae-ref-score-over-platform.svg", bbox_inches="tight", format="svg", transparent=True)

plot_acae_v_ref_bench("dhrystone", "DMIPS")
plot_acae_v_ref_bench("coremark", "CoreMark")

######################################################################################################################
def plot_acae_v_native_bench(benchmark: str, unit: str, ld_scale: bool):
    dataset_acae = pd.read_csv("acae-" + benchmark + ".csv")
    dataset_native = pd.read_csv("native-" + benchmark + ".csv")

    plt.figure()

    dataset_native_v_acae = pd.concat([dataset_acae, dataset_native], ignore_index=True)

    x_dataset_name: str = "iterations_ld" if ld_scale else "iterations"
    if ld_scale:
        dataset_native_v_acae["iterations_ld"] = np.log2(dataset_native_v_acae["iterations"]).astype(int)

    comp_score_bar_pl = sns.barplot(
        x=x_dataset_name,
        y="score",
        hue="platform",
        data=dataset_native_v_acae,
        palette="Set2"
    )

    comp_score_xticks = dataset_native_v_acae[x_dataset_name].unique() if ld_scale else np.arange(len(dataset_native_v_acae["iterations"].unique()))
    comp_score_xlabels = [rf"$2^{{{x}}}$" for x in comp_score_xticks] if ld_scale else dataset_native_v_acae["iterations"].unique()

    comp_score_bar_pl.set_xticks(comp_score_xticks)
    comp_score_bar_pl.set_xticklabels(comp_score_xlabels)

    if not ld_scale:
        plt.xticks(rotation=45)

    plt.xlabel("iterations")
    plt.ylabel("score")
    plt.legend(title="platform")
    plt.title(f"{benchmark} score (in {unit})")
    plt.savefig(f"pl-{benchmark}-acae-native-score-over-iter.svg", bbox_inches="tight", format="svg", transparent=True)

plot_acae_v_native_bench("dhrystone", "DMIPS", True)
plot_acae_v_native_bench("coremark", "CoreMark", False)

plt.show()