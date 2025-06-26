#include "idt.h"
#include "isr.h"
#include "../vga_text.h"
#include "../io.h"
#include <stdint.h>
#include <stdbool.h>

bool vectors[IDT_MAX_DESCRIPTORS];

extern uint64_t isr_stub_table[] __attribute__((aligned(8)));
extern uint64_t irq_stub_table[] __attribute__((aligned(8)));

idt_desc_t g_idt[IDT_MAX_DESCRIPTORS];

idtr_t g_idtr;

void idt_set_descriptor(uint8_t vector, uintptr_t isr, uint8_t flags, uint8_t ist)
{
  volatile idt_desc_t* descriptor = &g_idt[vector];

  descriptor->base_low       = isr & 0xFFFF;
  descriptor->base_mid       = (isr >> 16) & 0xFFFF;
  descriptor->base_high      = (isr >> 32) & 0xFFFFFFFF;
  descriptor->cs             = 0x08;
  descriptor->ist            = ist;
  descriptor->attributes     = flags;
  descriptor->rsv0           = 0;
}

void idt_assemble(uint16_t idt_count)
{
  g_idtr.base = (uintptr_t)&g_idt[0];
  g_idtr.limit = (uint16_t)sizeof(idt_desc_t) * idt_count - 1;
}

void idt_free_vector(uint8_t vector)
{
  idt_set_descriptor(vector, 0, 0, 0);
  vectors[vector] = false;
}

void idt_init(void)
{
  idt_assemble(IDT_MAX_DESCRIPTORS);

  for (uint8_t vector = 0; vector < IDT_CPU_EXCEPTION_COUNT; ++vector) {
    idt_set_descriptor(vector, isr_stub_table[vector], 0x8E, 0);

    vectors[vector] = true;
  }
}
