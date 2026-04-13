CWD := $(abspath $(dir $(lastword $(MAKEFILE_LIST))))
BENCHPATH := $(abspath $(CWD)/..)
ACAEPATH  := $(abspath $(BENCHPATH)/..)

COREMARKPATH := $(CWD)/coremark
COREMARKBMPATH := $(COREMARKPATH)/barebones

BMPATH   := $(ACAEPATH)/baremetal
OUTPATH  := $(CWD)/bin

# Compiler and flags
CC := arm-none-eabi-gcc
# preproc
CPPFLAGS += -I$(COREMARKPATH) -I$(COREMARKBMPATH) -DFLAGS_STR="\"$(CFLAGS)\""
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
	-g \
	-O0
# LDFLAGS +=
# LDLIBS +=

# Source files
COREMARKSRCS := core_list_join.c core_main.c core_matrix.c core_state.c core_util.c
COREMARKBMSRCS := core_portme.c
SRCS += \
	$(addprefix $(COREMARKPATH)/,$(COREMARKSRCS)) \
	$(addprefix $(COREMARKBMPATH)/,$(COREMARKBMSRCS))
vpath .c $(COREMARKPATH) $(COREMARKBMPATH)

include $(BMPATH)/baremetal.mk

# Target ELF
TARGET := $(OUTPATH)/coremark.elf

# Build the ELF
$(TARGET): $(SRCS)
	@mkdir -p $(OUTPATH)
	$(CC) $(CPPFLAGS) $(CFLAGS) $(LDFLAGS) $^ $(LDLIBS) -o $@

# Phony targets
.PHONY: all clean
all: $(TARGET)
clean:
	rm -rf $(OUTPATH)