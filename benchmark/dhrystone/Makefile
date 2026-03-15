# Directories
CWD := $(abspath $(dir $(lastword $(MAKEFILE_LIST))))
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
	-nostartfiles \
	-fno-unwind-tables \
	-fno-exceptions \
	-fno-asynchronous-unwind-tables \
	-fomit-frame-pointer \
	-nostdlib \
	-nodefaultlibs \
	-specs=nano.specs \
	\
	-std=gnu89 \
	-g0 \
	-O0
INCLUDES := -I$(CWD) -I$(BMPATH)
LDFLAGS := -T $(BMPATH)/link.ld -lgcc -lc_nano -lnosys

# Source files
BMSRCS := $(shell find $(BMPATH) -name '*.c')
SRCS := $(shell find $(CWD) -name '*.c') $(BMSRCS)

# Or if you want to be explicit
# BMSRCS := \
# 	boot/boot.c \
# 	boot/handlers.c \
# 	boot/stack.c \
# 	syscall_stubs.c
# SRCS := main.c $(addprefix $(BMPATH)/,$(BMSRCS))

# Target ELF
TARGET := $(OUTPATH)/dhrystone.elf

# if you ever need objs
# OBJS := $(addprefix $(OUTPATH)/,$(notdir $(SRCS:.c=.o)))
# $(OUTPATH)/%.o : %.c
#	$(CC) $(CFLAGS) $(XCFLAGS) $(COUT) $< $(OBJOUT) $@

# Build the ELF
$(TARGET): $(SRCS)
	@mkdir -p $(OUTPATH)
	$(CC) $(CFLAGS) $(INCLUDES) $^ $(LDFLAGS) -o $@

# Phony targets
.PHONY: all clean
all: $(TARGET)
clean:
	rm -rf $(OUTPATH)