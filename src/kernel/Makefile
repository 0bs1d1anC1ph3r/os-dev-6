.PHONY: all clean

ASM        ?= nasm
CC         ?= gcc
LD         ?= ld
OBJCOPY    ?= objcopy

BUILD_DIR   := $(abspath $(BUILD_DIR))
ENTRY_SRC   := kernel_entry.asm
KERNEL_C    := kernel.c
LINKER      := linker.ld

ENTRY_OBJ   := $(BUILD_DIR)/kernel_entry.o
KERNEL_OBJ  := $(BUILD_DIR)/kernel.o
KERNEL_ELF  := $(BUILD_DIR)/kernel.elf
KERNEL_BIN  := $(BUILD_DIR)/KERNEL.BIN

CFLAGS      := -m64 -ffreestanding -O2 -Wall -Wextra
LDFLAGS     := -m elf_x86_64 -T $(LINKER)

all: $(KERNEL_BIN)

# Ensure build directory exists
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Assemble the entry stub as ELF64 object
$(ENTRY_OBJ): $(ENTRY_SRC) | $(BUILD_DIR)
	$(ASM) -f elf64 $< -o $@

# Compile the C kernel into ELF64 object
$(KERNEL_OBJ): $(KERNEL_C) | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Link entry and kernel objects into a flat ELF using linker script
$(KERNEL_ELF): $(ENTRY_OBJ) $(KERNEL_OBJ) $(LINKER)
	$(LD) $(LDFLAGS) $(ENTRY_OBJ) $(KERNEL_OBJ) $(STDIO_OBJ) -o $@

# Convert ELF to raw binary
$(KERNEL_BIN): $(KERNEL_ELF)
	$(OBJCOPY) -O binary $< $@

clean:
	rm -rf $(ENTRY_OBJ) $(KERNEL_OBJ) $(KERNEL_ELF) $(KERNEL_BIN)
