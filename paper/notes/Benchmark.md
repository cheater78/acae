# Benchmark

## Ref Data
- Dhrystone, CoreMark, Embench for reference

Dhrystone(v2.1):
https://developer.arm.com/-/media/Arm%20Developer%20Community/PDF/Cortex-A%20R%20M%20datasheets/Arm%20Cortex-M%20Comparison%20Table_v3.pdf
1.25 DMIPS/MHz -> 125 DMIPS @100MHz

https://github.com/tomzbj/dhrystone_score
125 DMIPS @108MHz, 1.15 DMIPS/MHz (O3, closest to 100MHz, there are more(70-200MHz) to avg if needed)

from STM data sheet - STM32F411xC STM32F411xE
1.25 DMIPS/MHz, 125 DMIPS @100MHz

CoreMark:
https://developer.arm.com/-/media/Arm%20Developer%20Community/PDF/Cortex-A%20R%20M%20datasheets/Arm%20Cortex-M%20Comparison%20Table_v3.pdf
3.42 CoreMark/MHz -> 342 CoreMark @100MHz

https://documentation-service.arm.com/static/6331cea74c59b30b51770bca
Example Chart(Page 12) 478.266287 CoreMark @168MHz, 2.8468 CoreMark/MHz

https://www.eembc.org/viewer/?benchmark_seq=1435,1430,973,962,1528,1425,969,970
408.61 @123MHz, 3.3220 CoreMark/MHz
363.57 @123MHz, 2.9559 CoreMark/MHz
183.76 @80MHz, 2.2970 CoreMark/MHz
163.23 @80MHz, 2.0404 CoreMark/MHz
410.82 @123MHz, 3.3400	CoreMark/MHz
366.00 @123MHz, 2.9756 CoreMark/MHz
+ 2 more @70MHz

Embench(v0.5) -> HW M4 data:
https://github.com/embench/embench-iot-results
https://github.com/embench/embench-iot-results/blob/master/details/cortexm4-armv7m-gcc-9.2-o2.mediawiki
Size: 1.15
Speed: 16 @ 16MHz, 1 Speed/MHz
https://github.com/embench/embench-iot-results/blob/master/details/cortexm4-armv7m-gcc-9.2-os.mediawiki
Size: 1
Speed: 14.40, 0.9 Speed/MHz



### IotBench (Taiwan)
- validated on Quad ARM Cortex-A53 (probably too heavy)
- no clue how to run that - not documented
- uni website doesn't have any results - and I can't read chinese
https://ieeexplore.ieee.org/document/8802949

### IoTBench (China)
- only gem5 emulated results
- uni website - no results found - and I can't read chinese
https://www.sciencedirect.com/science/article/pii/S277248592300008X

### BenchIoT:
- validated on STM32F479I-Eval -> ARM Cortex M4(STM32F479NI -> 180Mhz)
- Benchmarks: SmartLight, Smart Thermostat, SmartLocker, FirmwareUpdater, ConnectedDisplay
- Metrics:
    - (Security)
    - Performance: Total Runtime cycles, sleep cycles
    - Memory: Total Flash Usage, Stack and Heap usage, Total RAM usage
    - (Energy: measured physically)
- IS A SECURITY BENCHMARK
    - compare Mbed-µVisor/OS/BM - we do BM only?
    - compatible without peripherals? - questionable in emu, try if time
https://ieeexplore.ieee.org/document/8809492
https://www.osti.gov/servlets/purl/1640943
- all authors from different institutes, no results found

### ARM Comparison Chart - Dhrystone and CoreMark
- M3: 1.25 DMIPS/MHz, 3.34 CoreMark/MHz
- M4: 1.25 DMIPS/MHz, 3.42 CoreMark/MHz
- see "Benchmark/resources/Arm Cortex-M Comparison Table_v3.pdf"
- https://developer.arm.com/-/media/Arm%20Developer%20Community/PDF/Cortex-A%20R%20M%20datasheets/Arm%20Cortex-M%20Comparison%20Table_v3.pdf

### Dhrystone
#### Git Dhrystone Scores Repo (M3 M4, STM32F4X)
https://github.com/tomzbj/dhrystone_score
#### STM32F41 - Dhrystone
- 125DMIPS(Dhrystone 2.1) - from STM datasheets
#### ARM Cortex M3 Boards - Dhrystone
https://www.eventiotic.com/eventiotic/files/Papers/URL/9cda7a70-2382-43e9-8adb-e33329d6bb4b.pdf
#### How do Dhrystone results in the Cortex-M3 "example" system compare with those in the Cortex-M4 Integration Kit?
https://developer.arm.com/documentation/ka001381/1-0/?lang=en

### CoreMark
#### ARM Docs - CoreMark
https://documentation-service.arm.com/static/6331cea74c59b30b51770bca

#### EEMBC scores (closest, not too great)
- filtered EEMBC scores(https://www.eembc.org/coremark/scores.php) by "M4"
- found some ATMEL and Microchip Techn. chips (73 - 123 MHz)
https://www.eembc.org/viewer/?benchmark_seq=1435,1430,973,962,1528,1425,969,970

### Embench (https://github.com/embench/embench-iot)(was run on HW M4)
https://github.com/embench/embench-iot-results

### WolfSSL (crypto benchmark with some HW M4 (and M3) board results)
https://www.wolfssl.com/docs/benchmarks/

### BEEBS (no results) - arxiv paper
https://github.com/mageec/beebs
https://arxiv.org/pdf/1308.5174
- M0 M3


