# Title
- MCU emulation
- for smart meters
- called it ACAE

# Motivation
- Why?

# Motivation - SM as edge
- currently SMs are measurement + reporting devices

BUT
- right at the data source
- have MCU

We asked: What else can we do with it?
- anomaly detection
- privacy preserving processing
- local analytics

to determine capabilities
# research questions and contributions

Core: How can we evaluate SM SW before deployment on real HW?

our approach:
- derive a representative SM MCU model (from publicly available specifications)
- implement an exec platform
    - reproducible
    - hw independent
    - performance resolving

- simulation
- directly test proposed applications

# SM hardware internals
- SMs are generally proprietary
- limited documentation
- focus on external attributes - necessary for installation (external interfaces and electrical characteristics)

available:
- 19 datasheets of MCUs intended for SM use
- 1 SM reference design by TI

# What does a SM MCU look like?
- dual core
    - metrology / compute
- SRAM, FLASH
- ADC, FPU

# The representative Model
- accumulated 19 datasheets and 1 ref design
- extracted common architectural attributes
- gathered each chip and its specs in a table
- selected representative values for each attribute
    - based on occurence and alignment within the dataset
- presented the model to a regulatory institute, which confirmed that representative

- comprises a representative SM MCU model
- not statistically proven average

# Simulation of the model
see slides

# ACAE - How it works
- uses QEMU
    - for functional emulation of an ARM Cortex-M4 board
- Q provides an instrum. IF -> instructions

- cyc acc model
    - instrument instructions
    - account clock cycles for executed instructions
    - provide accumulated clock cycles in the register

# Cycle Model
see slides

# Validation
- concept: compare ACAE to native HW
- benchmark both using Dhry and CoreMark
- run bare-metal for best reproducibility and minimal overhead
- identical source, compiler + config
- NO OPTIMIZATION
- measure performance of 3 repetitions -> determinism
- calculate the relative performance deviation

# Benchmarks
see slides

# Native HW
- used an ARM Cortex-M4 Blackpill board
- chip by ST-Microel.

- matches the relevant compute-core characteristics of our model:
    - Cortex-M4 architecture
    - 100 MHz clock freq
    - 128 KB SRAM
    - 512 KB Flash

# Results: Dhry
- measured score across iterations
- at low iteration counts, the score is lower
    - reproduced by ACAE
- as the workload becomes larger, the score approaches a stable value
- determinism
# Results: Dhry Comp
- compare at highest iter count
- ACAE overestimates performance by 74.6%
    - rel to native HW
- ref value deviates drastically
    - no compiler, optimization, libraries, benchmark configuration

# Results: CoreMark
- platforms are essentially stable across the evaluated iteration range
    - reproduced by ACAE
# Results: CoreMark Comp
- compare at highest iter count
- ACAE overestimates performance by 51.4%
    - rel to native HW
- ref value deviates drastically
    - no compiler, optimization, libraries, benchmark configuration

# assessment
1. Feasibility
    - A representative smart-meter MCU can be modeled and executed in QEMU
2. Instrumentation
    - modified QEMU can provide guest-visible cycle accounting
3. Reproducibility
    - Deterministic benchmark behavior reproduced
4. Performance trends
    - Dhrystone/CoreMark trends reproduced
5. Accuracy
    - Absolute performance remains systematically overestimated

# Limitations
- model validity
    - model is based on scarce public information
    - electrical SM only
    - cannot statistically represent the entire smart-meter market

- timing accuracy
    - cycle model is intentionally incomplete
    - primary limitation affecting absolute timing accuracy

- workload representation
    - only 2 benchmarks were evaluated
    - w/o compiler optimization
    - baremetal

- reference hardware
    - Single Cortex-M4 reference board

These limitations define the scope of my conclusions rather than invalidate the feasibility result

# Future Work
- expand validation methodology
    - incorporate and evaluate additional benchmarks
    - identify instruction classes and microarch effects - causing

- refine cycle model
    - introduce explicit costs for divisions, branches, and pipeline effects

- allow Optimized binaries
    - remove the current dependence on symbols and -O0 builds

- extend emulation environment
    - incorporate hardware accelerators / peripherals
    - for more complete SM emulation

# Conclusion
1. ACAE is feasible 
    - a representative smart-meter MCU can be implemented
    - as a QEMU-based emulation platform
    - with guest-visible cycle accounting

2. ACAE reproduces behavior, not yet absolute timing
    - systematically overestimates absolute performance
    - reproduces important execution trends observed on hardware

3. The cycle model is the main refinement target
    - validate broader set of benchmarks
    - improve cycle model according to workload

The broader goal is to make experimentation with future smart-meter functionality possible without requiring access to proprietary hardware.