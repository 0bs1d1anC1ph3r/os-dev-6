#include "idt.h"
#include "isr.h"
#include "vga_text.h"
#include <stdint.h>
#include <stdbool.h>

#define FLAG_SET(x, flag) x |= (flag)
#define FLAG_UNSET(x, flag) x &= ~(flag)

static bool vectors[IDT_MAX_DESCRIPTORS];

volatile idt_desc_t g_idt[IDT_MAX_DESCRIPTORS];

volatile idtr_t g_idtr;

extern uint64_t isr_stub_table[];

void idt_set_descriptor(uint8_t vector, uintptr_t isr, uint8_t flags, uint8_t ist)
{
    idt_desc_t* descriptor = &g_idt[vector];

    descriptor->base_low       = isr & 0xFFFF;
    descriptor->cs             = 0x08;
    descriptor->ist            = ist;
    descriptor->attributes     = flags;
    descriptor->base_mid       = (isr >> 16) & 0xFFFF;
    descriptor->base_high      = (isr >> 32) & 0xFFFFFFFF;
    descriptor->rsv0           = 0;
}

void idt_enable_gate(int interrupt)
  {
    FLAG_SET(g_idt[interrupt].attributes, IDT_DESCRIPTOR_PRESENT);
  }

void idt_disable_gate(int interrupt)
  {
    FLAG_UNSET(g_idt[interrupt].attributes, IDT_DESCRIPTOR_PRESENT);
  }

void idt_assemble(void)
  {
    g_idtr.base = (uintptr_t)&g_idt[0];
    g_idtr.limit = (uint16_t)sizeof(idt_desc_t) * IDT_MAX_DESCRIPTORS - 1;
  }



void idt_free_vector(uint8_t vector) {
    idt_set_descriptor(vector, 0, 0, 0);
    vectors[vector] = false;
}

void idt_init(void)
{
    idt_assemble();

    for (uint8_t vector = 0; vector < IDT_CPU_EXCEPTION_COUNT; ++vector) {
      idt_set_descriptor(vector, isr_stub_table[vector], IDT_DESCRIPTOR_EXCEPTION, 0);

      idt_enable_gate(vector);
    }

    idt_reload(&g_idtr);
}
