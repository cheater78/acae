BOOTPATH := $(abspath $(dir $(lastword $(MAKEFILE_LIST))))
BAREMETALPATH := $(abspath $(BOOTPATH)/..)
ACAEPATH  := $(abspath $(BAREMETALPATH)/..)

BOOTOUTPATH  := $(BOOTPATH)/bin

CC := arm-none-eabi-gcc
CPPFLAGS += -I$(BOOTPATH)
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

LDFLAGS += -T $(BOOTPATH)/baremetal.ld
# LDLIBS +=

BOOTSRCS += $(shell find $(BOOTPATH) -name '*.c')
vpath .c $(BOOTPATH)

# BOOTOBJS -> boot
BOOTOBJS := $(patsubst $(BOOTPATH)/%.c,$(BOOTOUTPATH)/%.o,$(BOOTSRCS))
# add to main obj dependecies
OBJS += $(BOOTOBJS)
# BOOTOBJS target
$(BOOTOUTPATH)/%.o: $(BOOTPATH)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

