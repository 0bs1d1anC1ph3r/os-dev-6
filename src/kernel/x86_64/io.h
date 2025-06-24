#pragma once
#include <stdint.h>

static inline void i686_outb(uint16_t port, uint8_t value)
{
  __asm__ volatile (
                    "outb %b0, %w1"
                    :
                    : "a"(value),
                      "Nd"(port)
                    :
                    );
}

static inline uint8_t i686_inb(uint16_t port)
{
  uint8_t value;
  __asm__ volatile (
                    "inb %w1, %b0"
                    : "=a"(value)
                    : "Nd"(port)
                    );
  return value;
}

static inline void i686_io_wait(void)
{
  __asm__ volatile (
                    "outb %%al, $0x80"
                    :
                    :
                    "a"(0)
                    );
}
