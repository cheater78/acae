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
	-nostartfiles \
	-fno-unwind-tables \
	-fno-exceptions \
	-fno-asynchronous-unwind-tables \
	-fomit-frame-pointer \
	-specs=nosys.specs \
	-specs=nano.specs\
	\
	-g \
	-O0
# LDFLAGS +=
# LDLIBS +=

# Source files
SRCS += $(shell find $(CWD) -name '*.c')
vpath .c $(CWD)

include $(BMPATH)/baremetal.mk

# Target ELF
TARGET := $(OUTPATH)/template.elf

# Build the ELF
$(TARGET): $(SRCS)
	@mkdir -p $(OUTPATH)
	$(CC) $(CPPFLAGS) $(CFLAGS) $(LDFLAGS) $^ $(LDLIBS) -o $@

.PHONY: all clean
all: $(TARGET)
clean:
	rm -rf $(OUTPATH)