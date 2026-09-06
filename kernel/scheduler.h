#ifndef SCHEDULER_H
#define SCHEDULER_H

void scheduler_init(void);

/*
 * Called from the timer interrupt.
 * Receives the current process ESP and
 * returns the ESP of the next process.
 */
unsigned int scheduler_switch(unsigned int current_esp);

#endif
