#include "vga_text.h"
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

void vga_puthex(uint64_t value)
{
  const char *hex_digits = "0123456789ABCDEF";
  int started = 0;

  vga_putc('0');
  vga_putc('x');

  for (int i = 60; i >= 0; i -= 4) {
    uint8_t nibble = (value >> i) & 0xF;
    if (nibble || started || i == 0) {
      vga_putc(hex_digits[nibble]);
      started = 1;
    }
  }
}

__attribute__((noreturn))
void isr_exception_handler(uint64_t vector, uint64_t error_code)
{

  vga_putc('\n');

  const char *vector_msg = "Vector: ";
  const char *error_code_msg = "Error Code: ";

  for (int i = 0; vector_msg[i] != '\0'; ++i) {
    vga_putc(vector_msg[i]);
  }

  vga_puthex(vector);
  vga_putc('\n');

  for (int i = 0; error_code_msg[i] != '\0'; ++i) {
    vga_putc(error_code_msg[i]);
  }

  vga_puthex(error_code);
  vga_putc('\n');

  if (vector < sizeof(exception_labels)/sizeof(*exception_labels)) {
    const char *label = exception_labels[vector];
    for (int i = 0; label[i] != '\0'; ++i)
      vga_putc(label[i]);
    vga_putc('\n');
  } else {
    const char *vector_error = "Unknown vector";
    for (int i = 0; vector_error[i] != '\0'; ++i) {
      vga_putc(vector_error[i]);
    }
    vga_putc('\n');
  }

  for (;;) {
    __asm__ volatile ("cli; hlt");
  }
}
