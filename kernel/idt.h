#ifndef IDT_H
#define IDT_H

void idt_init(void);
void idt_set_gate(int vector, unsigned int handler);

#endif
