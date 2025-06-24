ASM=nasm

SRC_DIR=src
BUILD_DIR=build
BOOTLOADER_INCLUDE_DIR=$(SRC_DIR)/bootloader/bootloader-includes

STAGE1_DIR = $(SRC_DIR)/bootloader/stage1
STAGE2_DIR = $(SRC_DIR)/bootloader/stage2
KERNEL_DIR = $(SRC_DIR)/kernel

STAGE1_BIN = $(BUILD_DIR)/STAGE1.BIN
STAGE2_BIN = $(BUILD_DIR)/STAGE2.BIN
KERNEL_BIN = $(BUILD_DIR)/KERNEL.BIN

FLOPPY_IMG = $(BUILD_DIR)/main_floppy.img

.PHONY: all clean always

all: floppy_image

floppy_image: bootloader kernel
	@echo "[+] - Creating blank floppy image"
	dd if=/dev/zero of=$(FLOPPY_IMG) bs=512 count=2880
	@echo "[+] - Copying STAGE1.BIN to boot sector (LBA 0)"
	dd if=$(STAGE1_BIN) of=$(FLOPPY_IMG) bs=512 count=1 conv=notrunc
	@echo "[+] - Copying STAGE2.BIN to LBA 1+"
	dd if=$(STAGE2_BIN) of=$(FLOPPY_IMG) bs=512 seek=1 conv=notrunc
	@echo "[+] - Copying KERNEL.BIN to LBA 20+"
	dd if=$(KERNEL_BIN) of=$(FLOPPY_IMG) bs=512 seek=3 conv=notrunc

bootloader: stage1 stage2

stage1: $(STAGE1_BIN)

$(STAGE1_BIN): always
	$(MAKE) -C $(STAGE1_DIR) \
		BUILD_DIR=$(abspath $(BUILD_DIR)) \
		BOOTLOADER_INCLUDE_DIR=$(abspath $(BOOTLOADER_INCLUDE_DIR))

stage2: $(STAGE2_BIN)
$(STAGE2_BIN): always
	$(MAKE) -C $(STAGE2_DIR) \
		BUILD_DIR=$(abspath $(BUILD_DIR)) \
		BOOTLOADER_INCLUDE_DIR=$(abspath $(BOOTLOADER_INCLUDE_DIR))

kernel:
	$(MAKE) -C $(KERNEL_DIR) \
	BUILD_DIR=$(abspath $(BUILD_DIR)) \

always:
	mkdir -p $(BUILD_DIR)

clean:
	$(MAKE) -C $(STAGE1_DIR) BUILD_DIR=$(abspath $(BUILD_DIR)) clean
	$(MAKE) -C $(STAGE2_DIR) BUILD_DIR=$(abspath $(BUILD_DIR)) clean
	$(MAKE) -C $(KERNEL_DIR) BUILD_DIR=$(abspath $(BUILD_DIR)) clean
	rm -rf $(BUILD_DIR)/*
