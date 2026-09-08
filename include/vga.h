#ifndef VGA_H
#define VGA_H

#include <stdint.h>

void vga_init(void);
void vga_putc(char c);
void vga_puts(const char *str);
void vga_puts_color(const char *str, uint8_t fg, uint8_t bg);
void print_number(uint32_t num);

#endif
