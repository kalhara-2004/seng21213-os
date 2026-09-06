#include "idt.h"

#define IDT_ENTRIES 256
#define KERNEL_CODE_SEGMENT 0x08

typedef struct {
    unsigned short offset_low;
    unsigned short selector;
    unsigned char zero;
    unsigned char type_attr;
    unsigned short offset_high;
} __attribute__((packed)) idt_entry_t;

typedef struct {
    unsigned short limit;
    unsigned int base;
} __attribute__((packed)) idt_ptr_t;

static idt_entry_t idt[IDT_ENTRIES];
static idt_ptr_t idt_ptr;

void idt_set_gate(int vector, unsigned int handler)
{
    idt[vector].offset_low = handler & 0xFFFF;
    idt[vector].selector = KERNEL_CODE_SEGMENT;
    idt[vector].zero = 0;

    /* Present, Ring 0, 32-bit interrupt gate */
    idt[vector].type_attr = 0x8E;

    idt[vector].offset_high =
        (handler >> 16) & 0xFFFF;
}

void idt_init(void)
{
    int i;

    for (i = 0; i < IDT_ENTRIES; i++) {
        idt[i].offset_low = 0;
        idt[i].selector = 0;
        idt[i].zero = 0;
        idt[i].type_attr = 0;
        idt[i].offset_high = 0;
    }

    idt_ptr.limit =
        (unsigned short)(sizeof(idt) - 1);

    idt_ptr.base =
        (unsigned int)&idt;

    /*
     * Do NOT enable interrupts here.
     * We will add valid IRQ handlers first.
     */
    __asm__ __volatile__(
        "lidt %0"
        :
        : "m"(idt_ptr)
    );
}
