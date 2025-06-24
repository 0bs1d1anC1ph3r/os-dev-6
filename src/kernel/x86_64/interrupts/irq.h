#pragma once
#include "idt.h"
#include <stdint.h>

void irq_init(void);
void irq_handler(uint64_t irq);
void pit_init(uint32_t hz);
void keyboard_handler(void);
void enable_keyboard_scan(void);
void set_kboard_scancode(uint8_t scancode_set);
