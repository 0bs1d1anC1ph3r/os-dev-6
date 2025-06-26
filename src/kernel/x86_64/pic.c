#include "pic.h"
#include "io.h"

void pic_remap(int offset1, int offset2) {
  i686_outb(0x20, 0x11);
  i686_outb(0xA0, 0x11);
  i686_outb(0x21, offset1);
  i686_outb(0xA1, offset2);
  i686_outb(0x21, 0x04);
  i686_outb(0xA1, 0x02);
  i686_outb(0x21, 0x01);
  i686_outb(0xA1, 0x01);
  i686_outb(0x21, 0x0);
  i686_outb(0xA1, 0x0);
}


void pic_mask_irq(uint8_t irq) {
    uint16_t port;
    uint8_t masks;

    if (irq < 8) {
        port = 0x21;
    }
    else {
        port = 0xA1;
        irq -= 8;
    }

    masks = i686_inb(port);
    masks |= (1 << irq);
    i686_outb(port, masks);
}

void pic_unmask_irq(uint8_t irq) {
    uint16_t port;
    uint8_t masks;

    if (irq < 8) {
        port = 0x21;
    }
    else {
        port = 0xA1;
        irq -= 8;
    }

    masks = i686_inb(port);
    masks &= ~(1 << irq);
    i686_outb(port, masks);
}
