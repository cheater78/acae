# Directories
CWD := $(abspath $(dir $(lastword $(MAKEFILE_LIST))))
COREMARKPATH := $(CWD)/coremark
COREMARKBMPATH := $(COREMARKPATH)/barebones
BENCHPATH := $(abspath $(CWD)/..)
BMPATH   := $(BENCHPATH)/bmlib
OUTPATH  := $(CWD)/bin

# Compiler and flags
CC := arm-none-eabi-gcc
CFLAGS := \
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
	-g \
	-O2
INCLUDES := -I$(COREMARKPATH) -I$(COREMARKBMPATH) -I$(BMPATH)
LDFLAGS := -T $(BMPATH)/link.ld

# Source files
vpath .c $(COREMARKPATH) $(COREMARKBMPATH) $(BMPATH)
BMSRCS := $(shell find $(BMPATH) -name '*.c')
COREMARKSRCS := core_list_join.c core_main.c core_matrix.c core_state.c core_util.c
COREMARKBMSRCS := core_portme.c
SRCS := \
	$(addprefix $(COREMARKPATH)/,$(COREMARKSRCS)) \
	$(addprefix $(COREMARKBMPATH)/,$(COREMARKBMSRCS)) \
	$(BMSRCS)

# Target ELF
TARGET := $(OUTPATH)/coremark.elf

# Build the ELF
$(TARGET): $(SRCS)
	@mkdir -p $(OUTPATH)
	$(CC) $(CFLAGS) $(INCLUDES) $^ $(LDFLAGS) -DFLAGS_STR="\"$(CFLAGS)\"" -o $@

# Phony targets
.PHONY: all clean
all: $(TARGET)
clean:
	rm -rf $(OUTPATH)