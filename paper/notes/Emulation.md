# Emulation

## Model
### Qemu
- trace instructions
### Parser - Instr. to Cycles
- model cycles / instructions
- accumulate cycles by parsing instructions
- if done well allegedly <5-10% measurement error

### The Cycle Model
- https://developer.arm.com/documentation/100166/0001/Programmers-Model/Instruction-set-summary/Table-of-processor-instructions?lang=en

## Background

### ARM Cycle-Accurate Simulator for free? - no
#### No
https://stackoverflow.com/questions/16198424/arm-cycle-accurate-simulator
https://www.systemonchips.com/cycle-accurate-simulation-challenges-for-arm-cortex-m4-processors/

#### ARM 
##### virtual HW (Functional, they're useless)
https://www.arm.com/products/development-tools/simulation/virtual-hardware

##### CPAKs (requested, but probably overkill - PhD work)
https://community.arm.com/support-forums/f/architectures-and-processors-forum/57406/cpaks-cycle-accurate-simulator-emulator-for-a-cortex-m4-processor-based-board
https://community.arm.com/support-forums/f/architectures-and-processors-forum/9271/cycle-accurate-simulator-emulator-for-a-cortex-m4-processor-based-board

#### actual HW approaches (reeally accurate and reliable - just not part of my work)
https://stackoverflow.com/questions/11530593/cycle-counter-on-arm-cortex-m4-or-cortex-m3

#### Cycle-Accurate Cortex-M Emulation (They wrote their own, thats inspiring .-.)
https://ieeexplore.ieee.org/document/7784555

#### Qemu? - NO, not cycle accurate -> use gem5
https://stackoverflow.com/questions/17454955/can-you-check-performance-of-a-program-running-with-qemu-simulator
##### Matlab SimuLink (Qemu)
https://www.mathworks.com/help/ecoder/armcortexm/ug/build-and-run-executable-on-arm-cortex-m-processors.html

#### Gem5
### Gem5 + m5
https://stackoverflow.com/questions/48944587/how-to-count-the-number-of-cpu-clock-cycles-between-the-start-and-end-of-a-bench
https://github.com/cirosantilli/linux-kernel-module-cheat/tree/aaf6b99c56062b7e5a485d1ac2584f3012610dd4?tab=readme-ov-file#gem5-run-benchmark

### Paper: gem5 benchmarking accuracy
https://ieeexplore.ieee.org/document/6322869
https://doi.org/10.1016/j.micpro.2022.104599

### Gem5 DOES NOT SUPPORT ARMv7 (ARM Cortex-M4)
- approximations possible

### Back to Qemu
- trace instructions
- model cycles / instructions
- accumulate cycles over parsing instructions
- if done well allegedly <5-10% measurement error
