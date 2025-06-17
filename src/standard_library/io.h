#pragma once
#include <stdint.h>

static inline void i686_outb(uint16_t port, uint8_t value) {
    __asm__ volatile (
        "outb %b0, %w1"
        :
        : "a"(value),
          "Nd"(port)
        :
    );
}
