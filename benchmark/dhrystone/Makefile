# Directories
CWD := $(abspath $(dir $(lastword $(MAKEFILE_LIST))))
BENCHPATH := $(abspath $(CWD)/..)
ACAEPATH  := $(abspath $(BENCHPATH)/..)

BMPATH   := $(ACAEPATH)/baremetal
BOOTPATH := $(BMPATH)/boot
OUTPATH  := $(CWD)/bin

# boot - defines the entrypoint
include $(BOOTPATH)/boot.mk

# preproc
CPPFLAGS += -I$(CWD)
ifdef ITERATIONS
CPPFLAGS += -DITERATIONS=$(ITERATIONS)
endif
CFLAGS += -g0 -O0
# LDFLAGS +=
# LDLIBS +=

# Source files
DHRYSRCS := $(shell find $(CWD) -name '*.c')
vpath .c $(CWD)

include $(BMPATH)/baremetal.mk

# dhrystone needs c89
DHRYCFLAGS := -std=gnu89

# DHRYOBJS -> dhrystone
DHRYOBJS := $(patsubst $(CWD)/%.c,$(OUTPATH)/%.o,$(DHRYSRCS))
# add to main obj dependecies
OBJS += $(DHRYOBJS)
# DHRYOBJS target
$(OUTPATH)/%.o: $(CWD)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CPPFLAGS) $(CFLAGS) $(DHRYCFLAGS) -c $< -o $@

# MAIN Target ELF
TARGET := $(OUTPATH)/dhrystone.elf

# MAIN TARGET
$(TARGET): $(OBJS)
	@mkdir -p $(OUTPATH)
	$(CC) $(CPPFLAGS) $(CFLAGS) $(LDFLAGS) $^ $(LDLIBS) -o $@

# Phony targets
.PHONY: all clean build run
clean:
	rm -rf $(OUTPATH)
list:
	@echo dhrystone
build: $(TARGET)

# RUN target
ifeq ($(PLATFORM),native)
# RUN on real HW
$(TARGET:.elf=.bin): $(TARGET)
	$(OBJCOPY) -O binary $< $@

run: $(TARGET:.elf=.bin)
	$(ACAEPATH)/serial/dfu.py
	dfu-util -a 0 -s 0x08000000:leave -D $<
	$(ACAEPATH)/serial/term.py
else ifeq ($(PLATFORM),acae)
# RUN on QEMU
run: $(TARGET)
	$(ACAEPATH)/emu/run.sh $(TARGET)
endif

all: clean build run