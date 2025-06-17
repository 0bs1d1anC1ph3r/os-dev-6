.PHONY: all clean

ASM        ?= nasm
CC         ?= gcc
LD         ?= ld
OBJCOPY    ?= objcopy

BUILD_DIR   := $(abspath $(BUILD_DIR))
LIB_DIR     := ../standard_library
ENTRY_SRC   := kernel_entry.asm
KERNEL_C    := kernel.c
LINKER      := linker.ld

ENTRY_OBJ   := $(BUILD_DIR)/kernel_entry.o
KERNEL_OBJ  := $(BUILD_DIR)/kernel.o
KERNEL_ELF  := $(BUILD_DIR)/kernel.elf
KERNEL_BIN  := $(BUILD_DIR)/KERNEL.BIN

# Compiler and linker flags
CFLAGS      := -nostdlib -nostdinc -I $(LIB_DIR) -m64 -ffreestanding -O2 -Wall -Wextra
LDFLAGS     := -m elf_x86_64 -T $(LINKER)

# Standard library sources and objects
LIB_SRC     := $(wildcard $(LIB_DIR)/*.c)
LIB_OBJ     := $(patsubst $(LIB_DIR)/%.c, $(BUILD_DIR)/%.o, $(LIB_SRC))

all: $(KERNEL_BIN)

# Ensure build directory exists
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Assemble kernel entry point
$(ENTRY_OBJ): $(ENTRY_SRC) | $(BUILD_DIR)
	$(ASM) -f elf64 $< -o $@

# Compile kernel C file
$(KERNEL_OBJ): $(KERNEL_C) | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Compile standard library C files
$(BUILD_DIR)/%.o: $(LIB_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Link everything into a kernel ELF file
$(KERNEL_ELF): $(ENTRY_OBJ) $(KERNEL_OBJ) $(LIB_OBJ) $(LINKER)
	$(LD) $(LDFLAGS) $(ENTRY_OBJ) $(KERNEL_OBJ) $(LIB_OBJ) -o $@

# Convert ELF to flat binary
$(KERNEL_BIN): $(KERNEL_ELF)
	$(OBJCOPY) -O binary $< $@

# Clean all build outputs
clean:
	rm -rf $(ENTRY_OBJ) $(KERNEL_OBJ) $(LIB_OBJ) $(KERNEL_ELF) $(KERNEL_BIN)
