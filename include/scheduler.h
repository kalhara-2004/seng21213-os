#ifndef SCHEDULER_H
#define SCHEDULER_H

void scheduler_init(void);
unsigned int scheduler_switch(unsigned int current_esp);
int scheduler_get_current(void);

#endif
