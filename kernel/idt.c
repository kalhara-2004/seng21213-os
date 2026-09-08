#include "idt.h"
#include <stdint.h>

#define IDT_ENTRIES 256
#define KERNEL_CODE_SEGMENT 0x08
extern void syscall_handler(void);
extern void syscall_stub(void);

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

void idt_set_gate(int vector, unsigned int handler,uint8_t flags)
{
   idt[vector].offset_low = handler & 0xFFFF;
    idt[vector].selector = KERNEL_CODE_SEGMENT;
    idt[vector].zero = 0;
    idt[vector].type_attr = flags;
    idt[vector].offset_high = (handler >> 16) & 0xFFFF;
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

	idt_set_gate(0x80, (unsigned int)syscall_stub, 0xEE);

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
