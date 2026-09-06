#include "pic.h"

#define PIC1_COMMAND 0x20
#define PIC1_DATA    0x21
#define PIC2_COMMAND 0xA0
#define PIC2_DATA    0xA1

#define PIC_EOI      0x20

static inline void outb(unsigned short port, unsigned char value)
{
    __asm__ __volatile__(
        "outb %0, %1"
        :
        : "a"(value), "Nd"(port)
    );
}

static inline unsigned char inb(unsigned short port)
{
    unsigned char value;

    __asm__ __volatile__(
        "inb %1, %0"
        : "=a"(value)
        : "Nd"(port)
    );

    return value;
}

static void io_wait(void)
{
    outb(0x80, 0);
}

void pic_remap(void)
{
  

    /* Start PIC initialization */
    outb(PIC1_COMMAND, 0x11);
    io_wait();

    outb(PIC2_COMMAND, 0x11);
    io_wait();

    /* Master IRQs -> IDT 32-39 */
    outb(PIC1_DATA, 0x20);
    io_wait();

    /* Slave IRQs -> IDT 40-47 */
    outb(PIC2_DATA, 0x28);
    io_wait();

    /* Tell master that slave is on IRQ2 */
    outb(PIC1_DATA, 0x04);
    io_wait();

    /* Tell slave its cascade identity */
    outb(PIC2_DATA, 0x02);
    io_wait();

    /* 8086 mode */
    outb(PIC1_DATA, 0x01);
    io_wait();

    outb(PIC2_DATA, 0x01);
    io_wait();

    /* Restore masks */
    outb(PIC1_DATA, 0xFF);
    outb(PIC2_DATA, 0xFF);
}

void pic_send_eoi(unsigned char irq)
{
    if (irq >= 8) {
        outb(PIC2_COMMAND, PIC_EOI);
    }

    outb(PIC1_COMMAND, PIC_EOI);
}

void pic_enable_irq(unsigned char irq)
{
    unsigned short port;outb(PIC1_DATA, 0xFF);
outb(PIC2_DATA, 0xFF);
    unsigned char value;

    if (irq < 8) {
        port = PIC1_DATA;
    } else {
        port = PIC2_DATA;
        irq -= 8;
    }

    value = inb(port);
    value &= (unsigned char)~(1u << irq);
    outb(port, value);
}
