# Directories
CWD := $(abspath $(dir $(lastword $(MAKEFILE_LIST))))
BENCHPATH := $(abspath $(CWD)/..)
ACAEPATH  := $(abspath $(BENCHPATH)/..)

BMPATH   := $(ACAEPATH)/baremetal
OUTPATH  := $(CWD)/bin

# Compiler and flags
CC := arm-none-eabi-gcc
# preproc
CPPFLAGS += -I$(CWD)
CFLAGS += \
	-mcpu=cortex-m4 \
	-mthumb \
	\
	-mfloat-abi=soft \
	-ffreestanding \
	-fno-unwind-tables \
	-fno-exceptions \
	-fno-asynchronous-unwind-tables \
	-fomit-frame-pointer \
	-specs=nosys.specs \
	-specs=nano.specs\
	\
	-g0 \
	-O0
# LDFLAGS +=
# LDLIBS +=

# Source files
DHRYSRCS += $(shell find $(CWD) -name '*.c')
vpath .c $(CWD)

include $(BMPATH)/baremetal.mk

DHRYTARGET := $(OUTPATH)/dhrystone.o
$(DHRYTARGET): $(DHRYSRCS)
	@mkdir -p $(OUTPATH)
	$(CC) $(CPPFLAGS) -std=gnu89 $(CFLAGS) $^ -o $@

# Target ELF
TARGET := $(OUTPATH)/dhrystone.elf

# Build the ELF
$(TARGET): $(SRCS) $(DHRYTARGET)
	@mkdir -p $(OUTPATH)
	$(CC) $(CPPFLAGS) $(CFLAGS) $(LDFLAGS) $^ $(LDLIBS) -o $@

# Phony targets
.PHONY: all clean
all: $(TARGET)
clean:
	rm -rf $(OUTPATH)