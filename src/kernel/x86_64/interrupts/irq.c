#include "irq.h"
#include "idt.h"
#include "../vga_text.h"
#include "../io.h"
#include <stdbool.h>

static bool vectors[IDT_MAX_DESCRIPTORS];
extern uint64_t irq_stub_table[];

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


void pit_init(uint32_t hz)
{
  uint16_t divisor = PIT_FREQUENCY / hz;
  i686_outb(PIT_COMMAND, 0x36);
  i686_outb(PIT_CHANNEL0, divisor & 0xFF);
  i686_outb(PIT_CHANNEL0, (divisor >> 8) & 0xFF);
}

void set_kboard_scancode(uint8_t set)
{
  i686_outb(0x60, 0xF0);
  i686_io_wait();

  while (i686_inb(0x60) != 0xFA)
    ;

  i686_outb(0x60, set);
  i686_io_wait();

  while (i686_inb(0x60) != 0xFA)
    ;
}

void keyboard_handler(void)
  {
    uint8_t scancode = i686_inb(0x60);
    vga_puthex(scancode);
  }

void timer_handle(void)
  {
    ++ticks;
  }

void irq_init(void)
  {
    for (uint8_t irq = 0; irq < 16; ++irq) {
      uint8_t vector = 32 + irq;
      idt_set_descriptor(vector, irq_stub_table[irq], IDT_DESCRIPTOR_EXTERNAL, 0);
      idt_enable_gate(vector);
      vectors[vector] = true;
    }
  }

void irq_handler(uint64_t irq)
{
  switch(irq) {
    case 32:
      timer_handle();
      break;
    case 33:
      keyboard_handler();
      break;
    default:
      vga_puts("IRQ: ");
      vga_puthex(irq);
      if (irq >= 32 && irq < 48) {
        vga_puts(" (");
        vga_puts(irq_names[irq - 32]);
        vga_puts(")");
      }
      vga_putc('\n');
  }

  if (irq >= 40) i686_outb(0xA0, 0x20);
  i686_outb(0x20, 0x20);
}
