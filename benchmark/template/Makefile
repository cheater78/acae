CWD := $(abspath $(dir $(lastword $(MAKEFILE_LIST))))
BENCHPATH := $(abspath $(CWD)/..)
ACAEPATH  := $(abspath $(BENCHPATH)/..)

OUTPATH := $(CWD)/bin

include $(ACAEPATH)/baremetal/baremetal.mk

###############################################################################
# Benchmark / Suite configuration
###############################################################################

# the folder containing this Makefile should be named like the benchmark / suite
BENCH_NAME := $(notdir $(CWD))

# Standalone (non suite) benchmark name
DEFAULT_TARGET := template

# Benchmark suite
# benchmark auto-discovery for suite directory
#SUITE_DIR := $(CWD)/embench/src
ifdef SUITE_DIR
	SUITE_TARGETS := $(notdir $(wildcard $(SUITE_DIR)/*))
endif
# alternative: explicit benchmark list
#SUITE_TARGETS := \
#	bench1 \
#	bench2 


# BENCHMARK may be supplied from command line
ifndef BENCHMARK
	ifdef SUITE_TARGETS
		$(error BENCHMARK must be specified. Available benchmarks: $(SUITE_TARGETS))
	else
		BENCHMARK := $(DEFAULT_TARGET)
	endif
else
	ifndef SUITE_TARGETS
		ifneq ($(BENCHMARK),$(DEFAULT_TARGET))
			$(warning Invalid BENCHMARK="$(BENCHMARK)". Expected "$(DEFAULT_TARGET)")
			BENCHMARK := $(DEFAULT_TARGET)
		endif
	else
		ifeq ($(filter $(BENCHMARK),$(SUITE_TARGETS)),)
			$(error Invalid BENCHMARK="$(BENCHMARK)". Available benchmarks: $(SUITE_TARGETS))
		endif
	endif
endif

###############################################################################
# Sources / Targets
###############################################################################

ifdef SUITE_TARGETS
	SRCDIR := $(SUITE_DIR)/$(BENCHMARK)
	TARGET := $(OUTPATH)/$(BENCHMARK).elf
else
	SRCDIR := $(CWD)
	TARGET := $(OUTPATH)/$(DEFAULT_TARGET).elf
endif

CPPFLAGS += -I$(SRCDIR)

ifdef ITERATIONS
	CPPFLAGS += -DITERATIONS=$(ITERATIONS)
endif

CFLAGS += -g0 -O0

# Source files
TEMPSRCS += $(shell find $(SRCDIR) -name '*.c')
vpath .c $(SRCDIR)

# TEMPOBJS -> template
TEMPOBJS := $(patsubst $(SRCDIR)/%.c,$(OUTPATH)/%.o,$(TEMPSRCS))
# add to main obj dependecies
OBJS += $(TEMPOBJS)

# TEMPOBJS target
$(OUTPATH)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@


###############################################################################
# MAIN TARGET
###############################################################################
$(TARGET): $(OBJS)
	@mkdir -p $(OUTPATH)
	$(CC) $(CPPFLAGS) $(CFLAGS) $(LDFLAGS) $^ $(LDLIBS) -o $@

###############################################################################
# Utility targets
###############################################################################
.PHONY: all clean build run list

list:
	ifdef SUITE_TARGETS
		@printf "%s\n" $(SUITE_TARGETS)
	else
		@echo $(DEFAULT_TARGET)
	endif

clean:
	rm -rf $(OUTPATH)

build: $(TARGET)

###############################################################################
# Run
###############################################################################

ifeq ($(PLATFORM),native)

$(TARGET:.elf=.bin): $(TARGET)
	$(OBJCOPY) -O binary $< $@

run: $(TARGET:.elf=.bin)
	$(ACAEPATH)/serial/dfu.py
	dfu-util -a 0 -s 0x08000000:leave -D $<
	$(ACAEPATH)/serial/term.py

else ifeq ($(PLATFORM),acae)

run: $(TARGET)
	$(ACAEPATH)/emu/run.sh $(TARGET)

endif

all: clean build run