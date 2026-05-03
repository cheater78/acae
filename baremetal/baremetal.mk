# all make paths for baremetal are to be defined here ONCE
BAREMETALPATH := $(abspath $(dir $(lastword $(MAKEFILE_LIST))))

BOOTPATH := $(BAREMETALPATH)/boot

BMPLATFORMBASEPATH := $(BAREMETALPATH)/platform
BMQEMUPATH := $(BMPLATFORMBASEPATH)/qemu
BMNATIVEPATH := $(BMPLATFORMBASEPATH)/native

BMPRINTFPATH := $(BAREMETALPATH)/printf

BMOUTPATH  := $(BAREMETALPATH)/bin

# Platform - default to qemu
V ?= 0
ifeq ($(V),1)
    BMPLATFORMPATH := $(BMNATIVEPATH)
else
    BMPLATFORMPATH := $(BMQEMUPATH)
endif

BMLIBCPPFLAGS := \
	-I$(BAREMETALPATH) \
	-I$(BMPRINTFPATH)

CC := arm-none-eabi-gcc
OBJCOPY := arm-none-eabi-objcopy
CPPFLAGS += $(BMLIBCPPFLAGS)
# optimization etc. defined in benchmark
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
	-specs=nano.specs
# LDFLAGS += # platform dependend
# LDLIBS +=

BMLIBSRCS += \
	$(shell find $(BAREMETALPATH) -name '*.c' -not -path '$(BOOTPATH)/*' -not -path '$(BMPLATFORMBASEPATH)/*')
vpath .c $(BAREMETALPATH) $(BMPRINTFPATH)

include $(BOOTPATH)/boot.mk
include $(BMPLATFORMPATH)/platform.mk

# BMLIBOBJS -> sys, dwt, stdlib, printf, _printchar
BMLIBOBJS += $(patsubst $(BAREMETALPATH)/%.c,$(BMOUTPATH)/%.o,$(BMLIBSRCS))
# add to main obj dependecies
OBJS += $(BMLIBOBJS)
# BMLIBOBJS target
$(BMOUTPATH)/%.o: $(BAREMETALPATH)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@