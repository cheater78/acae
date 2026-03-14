# Directories
TRACE_DIR := $(abspath $(dir $(lastword $(MAKEFILE_LIST))))
BASE_DIR  := $(abspath $(TRACE_DIR)/..)
QEMU_DIR  := $(BASE_DIR)/qemu
OUT_DIR   := $(TRACE_DIR)/bin

# Compiler and flags
CC := cc
CFLAGS := -O2 -fPIC -I$(QEMU_DIR)/include -I$(QEMU_DIR)/build $(shell pkg-config --cflags glib-2.0)
LDFLAGS := $(shell pkg-config --libs glib-2.0)

# Files
SRC := trace.c cycle_model.c
TARGET := $(OUT_DIR)/trace.so

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(SRC)
	@mkdir -p $(OUT_DIR)
	$(CC) $(CFLAGS) -shared $(SRC) -o $@ $(LDFLAGS)

clean:
	rm -rf $(OUT_DIR)