#include <test_header_include_all.h>
#include "x86_64/kernel_include_test_header.h"

void _kmain(void)
{
    vga_clear_screen();

    vga_puts("Hello World!");
    vga_putc('\n');

    pic_remap();
    idtr_t* g_idtr = idt_init();
    idt_reload(g_idtr);
    irq_init();

    __asm__ volatile ("sti");

    vga_puts("Interrupts enabled\n");

    for (;;)
        __asm__ volatile ("hlt");
}
