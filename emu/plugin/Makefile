PLUGINPATH := $(abspath $(dir $(lastword $(MAKEFILE_LIST))))
EMUPATH  := $(abspath $(PLUGINPATH)/..)
ACAEPATH  := $(abspath $(EMUPATH)/..)

QEMUPATH  := $(EMUPATH)/qemu
BMPATH   := $(ACAEPATH)/baremetal
BMQEMUPATH := $(BMPATH)/qemu
OUTPATH   := $(PLUGINPATH)/bin

# Compiler and flags
CC := cc
CFLAGS := -O2 -fPIC -I$(QEMUPATH)/include -I$(QEMUPATH)/build -I$(BMPATH) -I$(BMQEMUPATH) $(shell pkg-config --cflags glib-2.0) -I$(BMPATH)
LDFLAGS := $(shell pkg-config --libs glib-2.0)

# Files
SRC := $(shell find $(PLUGINPATH) -name '*.c')
TARGET := $(OUTPATH)/cyc_plugin.so

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(SRC)
	@mkdir -p $(OUTPATH)
	$(CC) $(CFLAGS) -shared $(SRC) -o $@ $(LDFLAGS)

clean:
	rm -rf $(OUTPATH)