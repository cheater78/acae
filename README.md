# ACAE – ARM Cycle Accounting Emulator
*A hardware-independent development and evaluation framework for embedded ARM-CM4-based Smart Meter software.*

---

## Overview

ACAE is a hardware-independent emulation framework for the development, validation, and performance evaluation of embedded ARM-CM4-based Smart Meter (SM) software.

Unlike conventional instruction-set emulators, ACAE augments functional emulation with a configurable cycle accounting model that estimates execution time based on executed instructions. This enables comparative performance evaluation against representative ARM Cortex-M4 hardware while remaining independent of proprietary Smart Meter platforms.

The project accompanies our research on Smart Meter hardware, software, and emulation and contains all artifacts required to reproduce the published evaluation results.

---

## Features

- ARM Cortex-M4 based Smart Meter emulation
- Cycle accounting timing model
- Dhrystone integration
- CoreMark integration
- Embench integration
- Automated benchmark execution
- Native hardware validation
- Automatic dataset generation
- Automatic plot generation
- Reproducible evaluation workflow

---

## Repository Structure

```
acae/
│
├── baremetal/         Bare-metal runtime library
├── benchmark/         Dhrystone, CoreMark and Embench
├── emu/               Modified QEMU sources and build scripts, cycle accounting QEMU plugin
├── eval/              Automated evaluation and plotting
├── serial/            UART relay and DFU utilities
├── paper/             Publications
└── setup.sh           Project setup
```

---

## Supported Platforms

### Host

- Ubuntu 24.04 (tested)
- Arch Linux (tested)

Other Linux distributions should work but are currently untested.

### Guest

- ARM Cortex-M4 (ARMv7 Thumb-2)
- 100 MHz
- 128 kB RAM
- 512 kB Flash

Reference hardware:

- STM32F411CEU6 (BlackPill Board)

---

# Installation

Clone the repository including all submodules.

```bash
git clone --recursive https://github.com/cheater78/acae.git
cd acae
./setup.sh
```

The setup script installs required dependencies, configures the tool-chains, and builds the customized ARM QEMU version used by ACAE.
The explicit dependencies are listed in setup.sh.

---

# Quick Start

Build and execute a benchmark manually.

```bash
PLATFORM=<acae|native> ITERATIONS=<n> \
make -C benchmark/<benchmark> clean build run
```

Example:

```bash
# running dhrystone on the emulator for 1024 iterations
PLATFORM=acae ITERATIONS=1024 \
make -C benchmark/dhrystone clean build run
```

---

# Reproducing the Paper

All experiments presented in the accompanying publications are generated through the automated evaluation framework.

## Generate benchmark data

Configure the desired evaluation at the bottom of, and execute

```bash
./eval/run.py
```

Generated datasets are stored in

```
eval/results/
```

---

## Generate figures

Configure the desired plots inside, and execute

```bash
./eval/plot.py
```

Generated figures are written to

```
eval/plots/
```

The datasets accompanying the publications are included in this repository.

---

# Native Hardware Setup

The reference STM32F411 (STM32F411CEU6, Black-Pill board) development board does not provide an integrated USB-to-UART interface.

For the reference evaluation platform, an ESP32 development board acts as a UART relay between the host PC and the target board.

Alternative USB-to-UART adapters may be used provided they expose an equivalent serial interface.

## Wiring

| ESP32 | STM32F411 |
|--------|-----------|
| GPIO16 (RX) | PA9 (TX) |
| GPIO17 (TX) | PA10 (RX) |
| GND | GND |
| 3V3 | 3V3 (through 1 kΩ resistor) |
---

## First Flash

The STM32 must be placed into DFU mode before the initial flash.

1. Hold the **BOOT0** button.
2. Press **RESET**.
3. Release **RESET**.
4. Release **BOOT0**.
5. Execute the desired benchmark.

Example:

```bash
ITERATIONS=1 make -C benchmark/template clean build run
```

After the initial flash, subsequent flashing is handled automatically through ACAE's UART-based DFU workflow once the benchmark has completed execution.

---

# Benchmark Framework

The project currently supports

- Dhrystone
- CoreMark
- Embench

All benchmarks share a common build interface and evaluation workflow.

Benchmark results are reported in the standardized form

```
Score: <value> <unit>
```

allowing the automated evaluation framework to extract benchmark results independent of the benchmark implementation.

NOTE: some benchmarks (embench) report time as their result. This will still be reported as Score. The unit will indicate this discrepancy.

---

# Build System

Every benchmark exposes a common Make interface.

## List available benchmarks

```bash
make -C benchmark/<suite|benchmark> list
# example:
make -C benchmark/embench list # will report a list of all benchmarks of the embench suite
make -C benchmark/dhrystone list # will report just dhrystone
```

---

## Build variables

```
BENCHMARK=<benchmark>
PLATFORM=<acae|native>
ITERATIONS=<count>
```

Example

```bash
BENCHMARK=nettle-aes \
PLATFORM=acae \
ITERATIONS=1024 \
make -C benchmark/embench clean build run
```

---

# Adding a Benchmark

A new benchmark can be added by copying

```
benchmark/template/
```

The required bare-metal runtime, startup code, linker configuration, and build infrastructure are linked automatically through

```
baremetal/baremetal.mk
```

Only benchmark-specific sources and build configuration need to be supplied in 

```
benchmark/<your-new-benchmark>/Makefile
```
The existing Makefile of template should help.

---

# Compiler Configuration

Benchmarks are compiled using

```
arm-none-eabi-gcc
```

with

```
-O0
-g0
```

to remain consistent with the validation methodology presented in our publications.

Additional architecture-specific compiler flags are configured automatically by

```
baremetal/baremetal.mk
```

---

# Future Work

Planned improvements include

- Improved cycle model calibration
- Support for optimized compiler configurations
- Additional benchmarks or suites
- Native USB CDC support

---

# Known Issues

## Dhrystone

Earlier versions occasionally reported

```
Increase the number of runs
```

or returned a score of zero due to instability in the previous inline timing interface.

This issue has been resolved through the dedicated `dwt_cyccnt()` implementation providing a real function symbol.

---

## Native Hardware

The initial flash requires manual entry into DFU mode.

Subsequent flashing is handled automatically through the UART relay interface.

---

# License

open source + free use

**TODO** proper license (include coremark's and embench's licence conditions + fork conditions)
