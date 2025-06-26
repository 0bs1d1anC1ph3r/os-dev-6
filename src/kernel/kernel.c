#include <test_header_include_all.h>
#include "x86_64/kernel_include_test_header.h"

void _kmain(void)
{
    vga_clear_screen();

    vga_puts("Hello World!");
    vga_putc('\n');

    pic_mask_irq(0xFF);
    pic_remap(0x20, 0x28);

    idtr_t* g_idtr = idt_init();
    irq_init();

    pit_init(100);
    idt_reload(g_idtr);

    pic_unmask_irq(1);

    __asm__ volatile ("sti");

    vga_puts("Interrupts enabled\n");

    for (;;)
        __asm__ volatile ("hlt");
}
