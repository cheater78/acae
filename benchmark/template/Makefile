CWD := $(abspath $(dir $(lastword $(MAKEFILE_LIST))))
BENCHPATH := $(abspath $(CWD)/..)
ACAEPATH  := $(abspath $(BENCHPATH)/..)

OUTPATH  := $(CWD)/bin

include $(ACAEPATH)/baremetal/baremetal.mk

# preproc
CPPFLAGS += -I$(CWD)
ifdef ITERATIONS
CPPFLAGS += -DITERATIONS=$(ITERATIONS)
endif
CFLAGS += -g0 -O0
# LDFLAGS +=
# LDLIBS +=

# Source files
TEMPSRCS += $(shell find $(CWD) -name '*.c')
vpath .c $(CWD)

# TEMPOBJS -> template
TEMPOBJS := $(patsubst $(CWD)/%.c,$(OUTPATH)/%.o,$(TEMPSRCS))
# add to main obj dependecies
OBJS += $(TEMPOBJS)
# TEMPOBJS target
$(OUTPATH)/%.o: $(CWD)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

# Target ELF
TARGET := $(OUTPATH)/template.elf

# MAIN TARGET
$(TARGET): $(OBJS)
	@mkdir -p $(OUTPATH)
	$(CC) $(CPPFLAGS) $(CFLAGS) $(LDFLAGS) $^ $(LDLIBS) -o $@

# Phony targets
.PHONY: all clean build run
clean:
	rm -rf $(OUTPATH)
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