# Title - Comparison of benchmarks on a smart meter prototype
- compared benchmarks
- on a smart meter prototype

# Back / recap
- Where does this come from?

# The representative SM MCU model
- SMs proprietary
- docs on internals unavailable

- pub data - 19 datasheets + 1 ref design
- derived a representative model
- presented and confirmed by regul. institute

- repr. != stat. proven avg.

# ACAE – an ARM Cycle Accounting Emulator
see slides - explain ACAE roughly

# Status quo
see slides

# Status quo [HIGHLIGHTED]
- Absolute timing systematically inaccurate
+ 2 benchmarks only
+ performance deviation:
    - 74.6% Dhrystone
    - 51.4% CoreMark
+ significantly apart
+ both: single aggregate benchmark result

+ Is the timing error systematic across workloads,
+ or does it depend on the computational characteristics of the workload?

# Extending ACAE’s Validation Methodology
- to find out
- we extended ACAE's Validation Methodology

# Validation Approach (adjusted)
see slides

# Embench
- open source benchmark suite
- 19 Benchmarks
- built for embedded systems
- aggregates realistic embedded workloads from previous works

+ replaced measurement harness with ACAEs

explicitly that means
# Measurement Methodology
- integrated:
    - cycle count register based timing
    - result reporting
    into Embench’s benchmark loop
- 2 cache warmup iterations
- ran for 1 to 4096 iterations

"performance deviation"!!!!
+ previously based on score
+ now on runtime
(explain formulas)

# Results
# Embench: nettle-aes
- nettle-aes as representative benchmark

- ACAE: 824 µs / iteration
- Native: 1546 µs / iteration
- Deviation: 46.7%

+ representative of the behavior observed across the Embench suite
- ACAE overestimates performance

+ iteration count independent!
    + following largest iteration count was used

# Overall Benchmark Results
(explain plot)
+ ACAE overestimates performance for ALL benchmarks
+ positive systematic error

+ Embench paints a more complete picture
+ Dhrystone produces a substantially larger deviation vs. Emb
+ CoreMark falls within
+ Embench exposes a spread from 20.4% to 55.9%

-> seem to have a systematic error +++ workload dependency

# Workload Characterization
- investigate the workload dependency further
- relate the measured deviations to the workload characterization
- published for Embench

- divide into branching, memory, compute
- suite contains considerably different workload compositions
(show a few)

- RISC-V dynamic instruction analysis
- not ARM

-> indication of possible dependency, rather than as direct evidence of a specific ARM processor-level error source

# Workload-Weighted Analysis
- combined the benchmark deviations
- with their published workload composition

- each measured deviation weighted by the corresponding fraction
    - of branching, memory, and compute
- resulting components sum to the original benchmark deviation

- Aggregating these components 
    - across the 19 workloads
    - gives three mean contributions

(show the mean deviations)
- compute category - largest contribution
- hints towards error origin
BUT
- contribution analysis
    - based on RISC-V data
- NOT a rigorous decomposition of processor timing error
- quantitative idea

# Assessment
(see slide)
+ ACAE underestimates the required processor cycles
    - for every evaluated workload

- workload-weighted analysis: compute - largest contribution
- experiment does not establish that conclusively
- hint: computational behavior deserves particular attention during cycle-model refinement

- Implications for the Cycle Model:
    - computational behavior: division
    - mem access + branching: pipeline effects
        - (flash wait states)

- prioritization for future investigation
- NOT conclusive proof

# Limitations
- workload representation
    + 19 Embench workloads != all embedded applications
    + no compiler optimizations
- reference hardware
    + Native reference is one Cortex-M4 platform
    + Hardware-specific memory/timing effects remain
- timing accuracy
    + Current cycle model remains simplified
- workload characterization data
    + Workload characterization from RISC-V not ARM

# Future Work
- Instruction tracing
    + acquire ARM-specific dynamic instruction composition
    + easy - ACAE already instruments instructions
- Processor-level measurements
    + Identify timing behavior of instruction classes
- Cycle-model refinement
    + add more detailed instruction costs
- Revalidation
    + after each change
        + Embench
        + additional workloads
        + optimization levels

# Conclusion
- ACAE shows systematic performance overestimation
    + all evaluated workloads exhibit performance overestimation.

- The magnitude is workload-dependent
    + Embench: large spread (20.4%–55.9%)
    + Embench: lower Mean (35.5%)

- The cycle model remains the main refinement target
    + performance overestimation also on Embench
    + seems: Compute-oriented workloads show the largest weighted contribution

Previously, we established that ACAE is feasible, but its timing model is inaccurate.
The unresolved question was whether this error is simply systematic or workload-dependent.
This work answers that question using 19 embedded workloads.