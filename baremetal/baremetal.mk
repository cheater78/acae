BAREMETALPATH := $(abspath $(dir $(lastword $(MAKEFILE_LIST))))
ACAEPATH  := $(abspath $(BAREMETALPATH)/..)

BMQEMUPATH := $(BAREMETALPATH)/qemu
BMNATIVEPATH := $(BAREMETALPATH)/native

BMPRINTFPATH := $(BAREMETALPATH)/printf

CPPFLAGS += \
	-I$(BAREMETALPATH) \
	-I$(BMPRINTFPATH) \
	-I$(BMQEMUPATH)

LDFLAGS += -T $(BAREMETALPATH)/baremetal.ld

SRCS += \
	$(shell find $(BAREMETALPATH) -name '*.c') \
	$(shell find $(BMPRINTFPATH) -name '*.c') \
	$(shell find $(BMQEMUPATH) -name '*.c')
vpath .c $(BAREMETALPATH) $(BMPRINTFPATH) $(BMQEMUPATH)
