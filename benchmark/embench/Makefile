CWD := $(abspath $(dir $(lastword $(MAKEFILE_LIST))))
BENCHPATH := $(abspath $(CWD)/..)
ACAEPATH  := $(abspath $(BENCHPATH)/..)

EMBPATH := $(CWD)/embench
EMBSUPPORTPATH := $(EMBPATH)/support
EMBCM4CHIPPATH := $(EMBPATH)/config/arm/chips/cortex-m4
EMBCM4BOARDPATH := $(EMBPATH)/config/arm/boards/acae-cortex-m4

OUTPATH := $(CWD)/bin

include $(ACAEPATH)/baremetal/baremetal.mk

###############################################################################
# Benchmark / Suite configuration
###############################################################################

# the folder containing this Makefile should be named like the benchmark / suite
BENCH_NAME := $(notdir $(CWD))

# Standalone (non suite) benchmark name
# DEFAULT_TARGET := template

# Benchmark suite
# benchmark auto-discovery for suite directory
SUITE_DIR := $(EMBPATH)/src
ifdef SUITE_DIR
	SUITE_TARGETS := $(notdir $(shell find $(SUITE_DIR) -mindepth 1 -maxdepth 1 -type d))
# define DEFAULT_TARGET as the first benchmark available
	DEFAULT_TARGET := $(firstword $(SUITE_TARGETS))
endif
# alternative: explicit benchmark list
#SUITE_TARGETS := \
#	aha-mont64 \
#	crc32 \
#	cubic \
#	edn \
#	huffbench


# BENCHMARK may be supplied from command line
ifndef BENCHMARK
	BENCHMARK := $(DEFAULT_TARGET)
endif

###############################################################################
# Sources / Targets
###############################################################################

ifdef SUITE_TARGETS
	SRCDIR := $(SUITE_DIR)/$(BENCHMARK)
	TARGET := $(OUTPATH)/$(BENCH_NAME)-$(BENCHMARK).elf
else
	SRCDIR := $(CWD)
	TARGET := $(OUTPATH)/$(DEFAULT_TARGET).elf
endif

CPPFLAGS += -I$(SRCDIR) \
	-I$(EMBSUPPORTPATH) \
	-I$(EMBCM4CHIPPATH) \
	-I$(EMBCM4BOARDPATH) \
	-DHAVE_BOARDSUPPORT_H=1 \
	-DITERATIONS=$(ITERATIONS)

ifdef ITERATIONS
	CPPFLAGS += -DITERATIONS=$(ITERATIONS)
endif

CFLAGS += -g0 -O0
LDLIBS += -lm

# Source files
EMBBENCHSRCS := $(shell find $(SRCDIR) -name '*.c')
vpath .c $(SRCDIR)
EMBSUPPORTSRCS := \
	$(EMBSUPPORTPATH)/beebsc.c \
	$(EMBSUPPORTPATH)/board.c \
	$(EMBSUPPORTPATH)/chip.c \
	$(EMBSUPPORTPATH)/main.c
vpath .c $(EMBSUPPORTPATH)

EMBOBJS := \
	$(patsubst $(EMBPATH)/%.c,$(OUTPATH)/%.o,$(EMBBENCHSRCS)) \
	$(patsubst $(EMBPATH)/%.c,$(OUTPATH)/%.o,$(EMBSUPPORTSRCS)) \
# add to main obj dependecies
OBJS += $(EMBOBJS)

# EMBOBJS target
$(OUTPATH)/%.o: $(EMBPATH)/%.c
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

ifdef SUITE_TARGETS
list:
	@printf "%s\n" $(SUITE_TARGETS)
else
list:
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