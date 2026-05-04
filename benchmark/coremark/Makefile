CWD := $(abspath $(dir $(lastword $(MAKEFILE_LIST))))
BENCHPATH := $(abspath $(CWD)/..)
ACAEPATH  := $(abspath $(BENCHPATH)/..)

COREMARKPATH := $(CWD)/coremark
COREMARKBMPATH := $(COREMARKPATH)/barebones

BMPATH   := $(ACAEPATH)/baremetal
BOOTPATH := $(BMPATH)/boot
OUTPATH  := $(CWD)/bin

# boot - defines the entrypoint
include $(BOOTPATH)/boot.mk

# preproc
CPPFLAGS += -I$(COREMARKPATH) -I$(COREMARKBMPATH) -DFLAGS_STR="\"$(CFLAGS)\""
ifdef ITERATIONS
CPPFLAGS += -DITERATIONS=$(ITERATIONS)
endif
CFLAGS += -g0 -O0
# LDFLAGS +=
# LDLIBS +=

# Source files
COREMARKSRCS := core_list_join.c core_main.c core_matrix.c core_state.c core_util.c
COREMARKBMSRCS := core_portme.c
CMSRCS := $(addprefix $(COREMARKPATH)/,$(COREMARKSRCS)) \
		$(addprefix $(COREMARKBMPATH)/,$(COREMARKBMSRCS))
vpath .c $(COREMARKPATH) $(COREMARKBMPATH)

include $(BMPATH)/baremetal.mk

# CMOBJS -> coremark
CMOBJS := $(patsubst $(CWD)/%.c,$(OUTPATH)/%.o,$(CMSRCS))
# add to main obj dependecies
OBJS += $(CMOBJS)
# CMOBJS target
$(OUTPATH)/%.o: $(CWD)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

# MAIN Target ELF
TARGET := $(OUTPATH)/coremark.elf

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