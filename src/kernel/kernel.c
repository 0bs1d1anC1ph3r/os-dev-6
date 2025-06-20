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
    i686_outb(0x20, 0x11);
    i686_outb(0xA0, 0x11);
    i686_outb(0x21, 0x20);
    i686_outb(0xA1, 0x28);
    i686_outb(0x21, 0x04);
    i686_outb(0xA1, 0x02);
    i686_outb(0x21, 0x01);
    i686_outb(0xA1, 0x01);
    i686_outb(0x21, 0x00);
    i686_outb(0xA1, 0x00);

    __asm__ volatile ("sti");

    for (;;)
        __asm__ volatile ("cli; hlt");
}
