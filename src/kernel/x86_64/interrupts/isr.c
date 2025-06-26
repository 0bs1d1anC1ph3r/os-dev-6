#include "../vga_text.h"
#include "isr.h"
#include <stdint.h>

static const char *exception_labels[] = {
    "[0x00] Divide by Zero Exception",
    "[0x01] Debug Exception",
    "[0x02] Unhandled Non-maskable Interrupt",
    "[0x03] Breakpoint Exception",
    "[0x04] Overflow Exception",
    "[0x05] Bound Range Exceeded Exception",
    "[0x06] Invalid Opcode/Operand Exception",
    "[0x07] Device Unavailable Exception",
    "[0x08] Double Fault",
    "[0x09] Coprocessor Segment Overrun",
    "[0x0A] Invalid TSS Exception",
    "[0x0B] Absent Segment Exception",
    "[0x0C] Stack-segment Fault",
    "[0x0D] General Protection Fault",
    "[0x0E] Page Fault",
    "[0x0F] Inexplicable Error",
    "[0x10] x87 Floating Exception",
    "[0x11] Alignment Check",
    "[0x12] Machine Check",
    "[0x13] SIMD Floating Exception",
    "[0x14] Virtualized Exception",
    "[0x15] Control Protection Exception",
    "[0x16] Inexplicable Error",
    "[0x17] Inexplicable Error",
    "[0x18] Inexplicable Error",
    "[0x19] Inexplicable Error",
    "[0x1A] Inexplicable Error",
    "[0x1B] Inexplicable Error",
    "[0x1C] Hypervisor Intrusion Exception",
    "[0x1D] VMM Communications Exception",
    "[0x1E] Security Exception",
    "[0x1F] Inexplicable Error"
};

extern const char *exception_labels[];

__attribute__((noreturn))
void isr_exception_handler(uint64_t vector, uint64_t error_code)
{

  vga_putc('\n');

  const char *vector_msg = "Vector: ";
  const char *error_code_msg = "Error Code: ";

  vga_puts(vector_msg);
  vga_puthex_8(vector);
  vga_putc('\n');

  vga_puts(error_code_msg);
  vga_puthex_64(error_code);
  vga_putc('\n');

  if (vector < sizeof(exception_labels)/sizeof(*exception_labels)) {
    const char *label = exception_labels[vector];
    vga_puts(label);
    vga_putc('\n');
  } else {
    const char *vector_error = "Unknown vector";
    vga_putc('\n');
    vga_puts(vector_error);
    vga_putc('\n');
  }

  for (;;) {
    __asm__ volatile ("cli; hlt");
  }
}
