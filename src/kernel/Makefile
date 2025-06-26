.PHONY: all clean

ASM          ?= nasm
CC           ?= gcc
LD           ?= ld
OBJCOPY      ?= objcopy

BUILD_DIR    := $(abspath $(BUILD_DIR))
LIB_DIR      := ../standard_library
K_INC_DIR	 := ./x86_64
K_INT_DIR	 := $(K_INC_DIR)/interrupts
ENTRY_SRC    := kernel_entry.asm
KERNEL_C     := kernel.c
LINKER       := linker.ld

ENTRY_OBJ    := $(BUILD_DIR)/kernel_entry.o
KERNEL_OBJ   := $(BUILD_DIR)/kernel.o
KERNEL_ELF   := $(BUILD_DIR)/kernel.elf
KERNEL_BIN   := $(BUILD_DIR)/KERNEL.BIN

# Compiler and linker flags
CFLAGS       := -nostdlib -nostdinc -I $(LIB_DIR) -m64 -ffreestanding -O2 -Wall -Wextra -g -O0 -mno-sse -mno-mmx
LDFLAGS      := -m elf_x86_64 -T $(LINKER) -g -O0

# Standard library sources and objects
LIB_SRC      := $(wildcard $(LIB_DIR)/*.c)
LIB_OBJ      := $(patsubst $(LIB_DIR)/%.c, $(BUILD_DIR)/%.o, $(LIB_SRC))

# Kernel include sources and objects
K_INC_SRC    := $(wildcard $(K_INC_DIR)/*.c)
K_ASM_SRC    := $(wildcard $(K_INC_DIR)/*.asm)

K_IN_C_SRC   := $(wildcard $(K_INT_DIR)/*.c)
K_IN_ASM_SRC := $(wildcard $(K_INT_DIR)/*.asm)

K_C_OBJ      := $(patsubst $(K_INC_DIR)/%.c, $(BUILD_DIR)/%.o, $(K_INC_SRC))
K_ASM_OBJ    := $(patsubst $(K_INC_DIR)/%.asm, $(BUILD_DIR)/%.o, $(K_ASM_SRC))

K_IN_C_OBJ   := $(patsubst $(K_INT_DIR)/%.c, $(BUILD_DIR)/%.o, $(K_IN_C_SRC))
K_IN_ASM_OBJ := $(patsubst $(K_INT_DIR)/%.asm, $(BUILD_DIR)/%.o, $(K_IN_ASM_SRC))

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

#Compile kernel include C files
$(BUILD_DIR)/%.o: $(K_INC_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

#Compile kernel include asm files
$(BUILD_DIR)/%.o: $(K_INC_DIR)/%.asm | $(BUILD_DIR)
	$(ASM) -f elf64 $< -o $@

#Compile kernel inturrupt C files
$(BUILD_DIR)/%.o: $(K_INT_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

#Compile kernel inturrupt asm files
$(BUILD_DIR)/%.o: $(K_INT_DIR)/%.asm | $(BUILD_DIR)
	$(ASM) -f elf64 $< -o $@

# Link everything into a kernel ELF file
$(KERNEL_ELF): $(ENTRY_OBJ) $(KERNEL_OBJ) $(LIB_OBJ) $(K_C_OBJ) $(K_ASM_OBJ) $(K_IN_C_OBJ) $(K_IN_ASM_OBJ)
	$(LD) $(LDFLAGS) $^ -o $@

# Convert ELF to flat binary
$(KERNEL_BIN): $(KERNEL_ELF)
	$(OBJCOPY) -O binary $< $@

# Clean all build outputs
 clean:
	rm -rf $(BUILD_DIR)/*.o $(KERNEL_ELF) $(KERNEL_BIN)
