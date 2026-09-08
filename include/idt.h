#ifndef IDT_H
#define IDT_H

#include <stdint.h>

void idt_init(void);
void idt_set_gate(int vector, unsigned int handler, uint8_t flags);

#endif
