# acae - an arm cycle accounting emulator
### Support
Host:
- ubuntu (24.04)
- archlinux (kernel: 7.0.3-arch1-2 (64-bit))

Guest:
- ARM Cortex M4 (ARMv7 Thumb/2) @100MHz

### Setup
```
git clone --recurse https://github.com/cheater78/acae.git
cd acea/
chmod +x setup.sh
./setup.sh
```

### Usage
Build and run a Benchmark manually:
```
make -C benchmark/<target_benchmark>/ clean build run
```

Automated Benchmark runs, with data export (to: eval/results/<platform>/<benchmark>.csv):
```
# select your benchmark(s) in run.py
eval/run.py
```

Build plots (requires exported bench data, then found at: eval/plots/*)
```
# select your plot(s) in plot.py
eval/plot.py
```

### Validation / Data
Our gathered data for validation is published alongside the project and can be found in [eval/results/][eval/results/].

### Extend the project
Adding a new Benchmark:
- use/copy benchmark/template
- adjust Makefile to target your source, baremetal lib and boot will be linked automatically (baremetal/baremetal.mk, baremetal/boot/boot.mk)

### WIP
Platform Native: USB CDC for UART
- baremetal/platform/native/usb
- baremetal/platform/native/platform.mk
- baremetal/platform/native/STM32CubeF4

Benchmark: embench
- benchmark/embench

### Known Issues
Dhrystone:
- reports no score or score 0 and tells you to "Increase the number of runs"
- std/times - randomly returns zero
- dirty fix: uncomment the debug printf in baremetal/bmstdlib.c times(), clean, build and run, comment out again, clean, build and run

First time build and flash to native HW:
- flash fails, because the chip is not in DFU mode
- fix: hold the BOOT0 (pin BOOT0 = HIGH) button during RESET
- subsequent DFU context switches are automated through the chips exit() function - therefor only available after main() has finished! 