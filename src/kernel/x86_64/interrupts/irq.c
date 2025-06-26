#include "irq.h"
#include "idt.h"
#include "../vga_text.h"
#include "../io.h"
#include <stdbool.h>
#include <stdint.h>

static volatile uint64_t ticks = 0;

#define PIT_CHANNEL0    0x40
#define PIT_COMMAND     0x43
#define PIT_FREQUENCY   1193182
#define PS2_SCANSET     0xF0
#define PS2_DATA_PORT   0x60

static const char* irq_names[] =
  {
    "Timer", "Keyboard", "Cascade", "COM2", "COM1", "LPT2", "Floppy", "LPT1",
    "RTC", "ACPI", "Unused", "Unused", "Mouse", "FPU", "Primary ATA", "Secondary ATA"
};

void ps2_flush_output_buffer(uint16_t timeout)
{
  while (i686_inb(0x64) & 1 && timeout > 0) {
    timeout--;
    i686_inb(0x60);
  }
}

void pit_init(uint32_t hz)
{
  uint16_t divisor = PIT_FREQUENCY / hz;
  i686_outb(PIT_COMMAND, 0x36);
  i686_outb(PIT_CHANNEL0, divisor & 0xFF);
  i686_outb(PIT_CHANNEL0, (divisor >> 8) & 0xFF);
}

static int ps2_wait_input(void)
{
  uint64_t timeout = 1000;
  while (--timeout) {
    if (!(i686_inb(0x64) & (1 << 1))) return 0;
  }
  return 1;
}

static int ps2_wait_output(void)
{
  uint64_t timeout = 1000;
  while (--timeout) {
    if (i686_inb(0x64) & (1 << 0)) return 0;
  }
  return 1;
}

void set_kboard_scancode(void)
{
  ps2_wait_input();
  i686_outb(0x60, 0xF0);
  ps2_wait_output();
  if (i686_inb(0x60) != 0xFA) {
    vga_puts("Failed to send 0xF0\n");
    return;
  }

  ps2_wait_input();
  i686_outb(0x60, 0x02);
  ps2_wait_output();
  if (i686_inb(0x60) != 0xFA) {
    vga_puts("Failed to send scancode set\n");
  }
}


void ps2_setup(void)
  {
    i686_outb(0x64, 0xAD);
    i686_outb(0x64, 0xA7);

    ps2_flush_output_buffer(1024);

    uint8_t status;

    ps2_wait_input();
    i686_outb(0x64, 0x20);
    ps2_wait_output();

    status = i686_inb(0x60);

    status &= ~0x30;
    status |= 0x01;

    ps2_wait_input();
    i686_outb(0x64, 0x60);
    ps2_wait_input();
    i686_outb(0x60, status);

    i686_outb(0x64, 0xAE);
    i686_outb(0x64, 0xA8);

    set_kboard_scancode();
  }

void keyboard_handler(void)
  {
    uint8_t scancode = i686_inb(0x60);
    vga_puthex_8(scancode);
  }

void timer_handler(void)
  {
    ++ticks;
  }

void irq_init(void)
  {
    for (uint8_t irq = 0; irq < 16; ++irq) {
      uint8_t vector = 32 + irq;
      idt_set_descriptor(vector, irq_stub_table[irq], 0x8E, 0);
      vectors[vector] = true;
    }
  }

void irq_handler(uint64_t irq)
{
  switch(irq) {
    case 0:
      timer_handler();
      break;
    case 1:
      keyboard_handler();
    break;
  default:
      vga_puts("IRQ: ");
      vga_puthex_64(irq);
      if (irq < 16) {
        vga_puts(" (");
        vga_puts(irq_names[irq]);
        vga_puts(")");
      }
      vga_putc('\n');
      break;
  }

  if (irq >= 8) i686_outb(0xA0, 0x20);
  i686_outb(0x20, 0x20);
}
