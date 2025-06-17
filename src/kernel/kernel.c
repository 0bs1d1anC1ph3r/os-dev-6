#include <test_header_include_all.h>

void _kmain(void)
{
    vga_clear_screen();
    uint16_t *vga = (uint16_t*)0xB8000;
    const char *msg = "KERNEL!!";
    for (int i = 0; msg[i]; ++i)
        vga[i] = (0x0F << 8) | msg[i];


    while (1)
        __asm__ volatile ("hlt");
}
