#include "pic.h"
#include "io.h"

void pic_remap(void)
{
    uint8_t a1 = i686_inb(PIC1_DATA);
    uint8_t a2 = i686_inb(PIC2_DATA);

    i686_outb(PIC1_COMMAND, ICW1_INIT | ICW1_ICW4);
    i686_io_wait();
    i686_outb(PIC2_COMMAND, ICW1_INIT | ICW1_ICW4);
    i686_io_wait();

    i686_outb(PIC1_DATA, 0x20);
    i686_io_wait();
    i686_outb(PIC2_DATA, 0x28);
    i686_io_wait();

    i686_outb(PIC1_DATA, 0x04);
    i686_io_wait();
    i686_outb(PIC2_DATA, 0x02);
    i686_io_wait();

    i686_outb(PIC1_DATA, ICW4_8086);
    i686_io_wait();
    i686_outb(PIC2_DATA, ICW4_8086);
    i686_io_wait();

    i686_outb(PIC1_DATA, a1);
    i686_outb(PIC2_DATA, a2);
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
