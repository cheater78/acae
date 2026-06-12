# Methodology

## SM Spec Acquisition
- Searched publicly available data:
    - from chip vendors:
        - 1 SM reference design from Ti
        - 19 chip data sheets (suggested for SM use or mentioned in SM articles -> see evidence of use folder)
    - from SM vendors:
        - public data sheets contain install and use information only
- Requested specs from SM vendors (used chips / architectural info)
    - 8 requests based on an article ranking the biggest SM vendors
    - no response

## SM Model
### Compute Performance
- collected chips in a table
- decided to focus on the compute core in multicore systems -> disregarding the metrology core (this distinction is done in many chip designs)
    - Our goal is to estimate the performance of these devices for increased cryptographic use
- picked the most "average / recent / actually used(PTB)"
- concluded the ARM Cortex M4 Spec as broadly appearing from various chip vendors
    - Fcpu: 100Mhz
    - Mem: SRAM
- confidential exchange with PTB
    - non-binding verification of out SM chip model
### Networking Performance
- search and collection of Com standards used in SMs:
    - PTB(public TS): one wired serial, one DIN wireless (licensed spec sheet, no access) -> both ancient tech
    - WiFi, BT, ZigBee, PLC, RF: LoRa
    - TODO

## MCU Emulation

### Planning
- searched for publicly available emulators -> generic ones + vendor published:
    - functional emulators -> do not allow for performance measurements -> no pipeline/cycle emulation:
        - Qemu -> strictly ordered op translation and execution
        - Rnode -> event based, no strictly ordered op translation and execution
        - gem5 -> does not support ARMv7 (32bit), though there has been research for cycle estimations
    - cycle accurate emulators:
        - generally not available for ARM publically
        - CPAK model was requested, but as found in preceding searches handling and delivery delays do not fit our time frame
        - SystemC: hardware emulator, requires architectural models by vendors -> not given, setup and tuning efforts way beyond our scope

- determined requirements:
    - compute emulation -> no peripherals
    - no hardware accelerators (for now?) -> no FPU in Model, crypto would be relevant
    - just fine-grained enough for performance estimations

- decided for a cycle accounting functional emulation using Qemu:
    - the functional emulation allows for fast execution
    - qemu translates every mcu native instruction to a host native instruction
    - qemu allows for external plugins, binding to an op translation cb
        - not using the debug instruction log -> write to file massively slows execution, logs are huge(multiple GB), we don't care about individual ops
    - a cycle model is used to accumulate the cycle count during runtime
    - this accumulation is an estimation of real hardware cycles passed for a task
    - claim: the quality of the cycle estimation will depend on the cycle model used
    - decided for baremetal tasks:
        - PTB suggested that dedicated firmware and OS' are used somewhat equally
        - baremetal provides a better raw performance measurement -> no scheduling overhead

### Implementation
- baremetal
- modding of benchmarks
- provision of system functions
- stdlib replacement
- newlib nano, for minimal links (-lc_nano, -lnosys) -> outdated
- no generic stdlib support -> couldn't get it working

### Cycle model:
- background:
    - 3 stage pipline
    - many instr. require 1 or 1+P cycles
    - synchronous cache
    - 100MHz only though clk multiplier -> mem runs on clk
        - 100Mhz with 25Mhz xtal -> 3 memstalls (1 access op + 3 for synchronization)

- default to 1 cycle per instruction (as most are)
- memory stalls approxated as: +1 cycle -> would be 3 for 100Mhz with 25MHz clk or 0 when cached

- optional improvements:
    - division ops (with average value)
    - other heavier ops
    - hazard / pipline refill detection -> keep temporal model of the pipeline in mem, detect hazards -> account for refills properly
    - cache memory model -> keep a cache model in memory -> detect cache hits and misses -> account for mem stalls properly

### Benchmarking
- edited benchmarks outside their critical section for compatibility and convenience
#### coremark
- score is iteration/second
#### dhrystone
- score in Dhrys per second
- this then is referenced to the VAX 11/780 baseline (1757) -> DMIPS = DPS / 1757
- finally divided by MHz to give DMIPS/MHz