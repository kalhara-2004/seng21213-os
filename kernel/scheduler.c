#include "scheduler.h"
#include "process.h"

static int current_index = 0;

void scheduler_init(void)
{
    current_index = 0;
}

unsigned int scheduler_switch(unsigned int current_esp)
{
    pcb_t *table = process_get_table();
    int i;
    int next;

    /*
     * Save the current process's stack.
     */
    if (table[current_index].state == PROCESS_RUNNING) {
        table[current_index].esp = current_esp;
        table[current_index].state = PROCESS_READY;
    }

    /*
     * Round-Robin:
     * search from the process after the current one.
     */
    for (i = 1; i <= MAX_PROCESSES; i++) {

        next = (current_index + i) % MAX_PROCESSES;

        if (table[next].state == PROCESS_READY) {

            current_index = next;
            table[current_index].state = PROCESS_RUNNING;

            return table[current_index].esp;
        }
    }

    /*
     * Nothing else is ready.
     * Continue running the current process.
     */
    table[current_index].state = PROCESS_RUNNING;

    return current_esp;
}
