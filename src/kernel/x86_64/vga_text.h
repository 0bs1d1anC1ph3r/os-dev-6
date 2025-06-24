#pragma once
#include <stdint.h>

void     vga_put_char    (int x, int y, char c);
void     vga_put_color   (int x, int y, uint8_t color);
char     vga_get_chr     (int x, int y);
uint8_t  vga_get_color   (int x, int y);
void     vga_set_cursor  (int x, int y);
void     vga_clear_screen(void);
void     vga_scroll_back (int lines);
void     vga_putc        (char c);
void     vga_puthex      (uint64_t value);
void     vga_puts        (const char *string);
