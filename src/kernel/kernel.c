#include <test_header_include_all.h>
#include "x86_64/kernel_include_test_header.h"

const char *msg_test = " Test!";

void test_divide_by_zero(void)
{
    volatile int a = 1;
    volatile int b = 0;
    volatile int c = a / b;
}

void _kmain(void)
{
    vga_clear_screen();

    const char *msg = "Hello World!";
    for (int i = 0; msg[i] != '\0'; ++i) {
        vga_putc(msg[i]);
    }

    idt_init();

    test_divide_by_zero();

    const char *msg_2 = " Works!";
    for (int i = 0; msg_2[i] != '\0'; ++i) {
        vga_putc(msg_2[i]);
    }

    for (;;)
        __asm__ volatile ("hlt");
}
