#include <test_header_include_all.h>
#include "x86_64/kernel_include_test_header.h"

void _kmain(void)
{
    vga_clear_screen();

    const char *msg = "Hello World!";
    for (int i = 0; msg[i] != '\0'; ++i) {
        vga_putc(msg[i]);
    }

    idt_init();

    for (;;)
        __asm__ volatile ("hlt");
}
