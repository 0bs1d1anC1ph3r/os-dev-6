#include "vga_text.h"
#include "io.h"
#include <stdint.h>

const unsigned SCREEN_WIDTH  = 80;
const unsigned SCREEN_HEIGHT = 25;
const uint8_t DEFAULT_COLOR  = 0x07;

uint8_t* screen_buffer = (uint8_t*)0xB8000;
int screen_x = 0, screen_y = 0;

void vga_put_char(int x, int y, char c)
  {
    screen_buffer[2 * (y * SCREEN_WIDTH + x)] = c;
  }

void vga_put_color(int x, int y, uint8_t color)
  {
    screen_buffer[2 * (y * SCREEN_WIDTH + x) + 1] = color;
  }

char vga_get_char(int x, int y)
  {
    return screen_buffer[2 * (y * SCREEN_WIDTH + x)];
  }

uint8_t vga_get_color(int x, int y)
  {
    return screen_buffer[2 * (y * SCREEN_WIDTH + x) + 1];
  }

void vga_set_cursor(int x, int y)
  {
    int pos = y * SCREEN_WIDTH + x;

    i686_outb(0x3D4, 0x0F);
    i686_outb(0x3D5, (uint8_t)(pos & 0xFF));
    i686_outb(0x3D4, 0x0E);
    i686_outb(0x3D5, (uint8_t)((pos >> 8) & 0xFF));
  }

void vga_clear_screen()
  {
    for (unsigned y = 0; y < SCREEN_HEIGHT; ++y) {
      for (unsigned x = 0; x < SCREEN_WIDTH; ++x) {
        vga_put_char(x, y, ' ');
        vga_put_color(x, y, DEFAULT_COLOR);
      }
    }
    screen_x = 0;
    screen_y = 0;
    vga_set_cursor(screen_x, screen_y);
  }

void vga_scroll_back(int lines)
{
  if ((unsigned)lines > SCREEN_HEIGHT) return;

  for (unsigned y = lines; y < SCREEN_HEIGHT; ++y) {
    for (unsigned x = 0; x < SCREEN_WIDTH; ++x) {
      vga_put_char(x, y - lines, vga_get_char(x, y));
      vga_put_color(x, y - lines, vga_get_color(x, y));
    }
  }

  for (unsigned y = SCREEN_HEIGHT - lines; y < SCREEN_HEIGHT; ++y) {
    for (unsigned x = 0; x < SCREEN_WIDTH; ++x) {
      vga_put_char(x, y, ' ');
      vga_put_color(x, y, DEFAULT_COLOR);
    }
  }

  screen_y -= lines;
  if (screen_y < 0) screen_y = 0;
}

void vga_putc(char c)
{
  switch (c) {
  case '\n':
    screen_x = 0;
    screen_y++;
    break;
  case '\t':
    for (int i = 0; i < 4 - (screen_x % 4); ++i) {
      vga_putc(' ');
    }
    break;
  case '\r':
    screen_x = 0;
    break;
  default:
    vga_put_char(screen_x, screen_y, c);
    vga_put_color(screen_x, screen_y, DEFAULT_COLOR);
    screen_x++;
    break;
  }

  if ((unsigned)screen_x >= SCREEN_WIDTH) {
    screen_y++;
    screen_x = 0;
  }

  if ((unsigned)screen_y >= SCREEN_HEIGHT) {
    vga_scroll_back(1);
  }

  vga_set_cursor(screen_x, screen_y);
}

void vga_puthex_8(uint8_t value)
{
  const char *hex_digits = "0123456789ABCDEF";

  vga_putc('0');
  vga_putc('x');
  vga_putc(hex_digits[(value >> 4) & 0xF]);
  vga_putc(hex_digits[value & 0xF]);
}

void vga_puthex_64(uint64_t value)
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

void vga_puts(const char *string)
  {
    for (int i = 0; string[i] != '\0'; ++i)
      vga_putc(string[i]);
  }
