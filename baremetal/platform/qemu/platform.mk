# vars defined in baremetal.mk

BMLIBCPPFLAGS += -I$(BMQEMUPATH)
BMLIBSRCS += $(shell find $(BMQEMUPATH) -name '*.c')
LDFLAGS += -L$(BMQEMUPATH) -T$(BMQEMUPATH)/link.ld

vpath .c $(BMQEMUPATH)