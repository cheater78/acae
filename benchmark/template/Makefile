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
CFLAGS += -g0 -O0
# LDFLAGS +=
# LDLIBS +=

# Source files
TEMPSRCS += $(shell find $(CWD) -name '*.c')
vpath .c $(CWD)

include $(BMPATH)/baremetal.mk

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
.PHONY: all clean
all: $(TARGET)
clean:
	rm -rf $(OUTPATH)