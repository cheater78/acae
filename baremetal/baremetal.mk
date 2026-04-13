BAREMETALPATH := $(abspath $(dir $(lastword $(MAKEFILE_LIST))))
ACAEPATH  := $(abspath $(BAREMETALPATH)/..)
BOOTPATH := $(BAREMETALPATH)/boot

BMQEMUPATH := $(BAREMETALPATH)/qemu
BMNATIVEPATH := $(BAREMETALPATH)/native

BMPRINTFPATH := $(BAREMETALPATH)/printf

BMOUTPATH  := $(BAREMETALPATH)/bin

BMLIBCPPFLAGS := \
	-I$(BAREMETALPATH) \
	-I$(BMPRINTFPATH) \
	-I$(BMQEMUPATH)
# add to main prepoc flags
CPPFLAGS += $(BMLIBCPPFLAGS)
# CFLAGS += # core defined in boot, optimization etc. defined in benchmark
# LDFLAGS +=
# LDLIBS +=

BMLIBSRCS := \
	$(shell find $(BAREMETALPATH) -name '*.c' -not -path '$(BOOTPATH)/*') \
	$(shell find $(BMPRINTFPATH) -name '*.c') \
	$(shell find $(BMQEMUPATH) -name '*.c')
vpath .c $(BAREMETALPATH) $(BMPRINTFPATH) $(BMQEMUPATH)

# BMLIBOBJS -> sys, dwt, stdlib, printf, _printchar
BMLIBOBJS := $(patsubst $(BAREMETALPATH)/%.c,$(BMOUTPATH)/%.o,$(BMLIBSRCS))
# add to main obj dependecies
OBJS += $(BMLIBOBJS)
# BMLIBOBJS target
$(BMOUTPATH)/%.o: $(BAREMETALPATH)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@