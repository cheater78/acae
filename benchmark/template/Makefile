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

$(TARGET:.elf=.bin): $(TARGET)
	$(OBJCOPY) -O binary $< $@

# Phony targets
.PHONY: all clean flash
all: $(TARGET)
clean:
	rm -rf $(OUTPATH)
flash: $(TARGET:.elf=.bin)
	dfu-util -a 0 -s 0x08000000:leave -D $<
	