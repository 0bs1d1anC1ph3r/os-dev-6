.PHONY: all clean

ASM?=nasm
BUILD_DIR?=build
BOOTLOADER_INCLUDE_DIR?=./bootloader-includes

all: $(BUILD_DIR)/STAGE1.BIN

$(BUILD_DIR)/STAGE1.BIN: stage1.asm | $(BUILD_DIR)
	$(ASM) -i $(BOOTLOADER_INCLUDE_DIR) -f bin stage1.asm -o $@

$(BUILD_DIR):
	mkdir -p $@

clean:
	rm -f $(BUILD_DIR)/STAGE1.BIN
