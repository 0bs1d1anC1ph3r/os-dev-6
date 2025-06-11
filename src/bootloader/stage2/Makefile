.PHONY: all clean

ASM?=nasm
BUILD_DIR?=build
BOOTLOADER_INCLUDE_DIR?=./bootloader-includes

all: $(BUILD_DIR)/STAGE2.BIN

$(BUILD_DIR)/STAGE2.BIN: stage2.asm | $(BUILD_DIR)
	$(ASM) -i $(BOOTLOADER_INCLUDE_DIR) -f bin stage2.asm -o $@

$(BUILD_DIR):
	mkdir -p $@

clean:
	rm -f $(BUILD_DIR)/STAGE2.BIN
