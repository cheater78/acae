# vars defined in baremetal.mk

BOOTOUTPATH := $(BOOTPATH)/bin

CPPFLAGS += -I$(BOOTPATH)
LDFLAGS += -L$(BOOTPATH) # platform dependend
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

