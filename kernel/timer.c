#include "timer.h"
#include "pic.h"

#define PIT_CHANNEL0 0x40
#define PIT_COMMAND  0x43
#define PIT_FREQUENCY 1193182

static volatile unsigned int timer_ticks = 0;

static inline void outb(unsigned short port, unsigned char value)
{
    __asm__ __volatile__(
        "outb %0, %1"
        :
        : "a"(value), "Nd"(port)
    );
}

void timer_init(unsigned int frequency)
{
    unsigned int divisor;

    if (frequency == 0) {
        frequency = 100;
    }

    divisor = PIT_FREQUENCY / frequency;

    /* Channel 0, low/high byte, mode 3, binary */
    outb(PIT_COMMAND, 0x36);

    outb(PIT_CHANNEL0, (unsigned char)(divisor & 0xFF));
    outb(PIT_CHANNEL0, (unsigned char)((divisor >> 8) & 0xFF));
}

void timer_handler(void)
{
    timer_ticks++;

    /*
     * Scheduler will eventually be called here.
     */

    pic_send_eoi(0);
}

unsigned int timer_get_ticks(void)
{
    return timer_ticks;
}
