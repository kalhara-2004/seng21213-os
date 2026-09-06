#include "process.h"

static pcb_t process_table[MAX_PROCESSES];
static int process_count = 0;
static int next_pid = 1;

void process_init(void)
{
    int i;

    for (i = 0; i < MAX_PROCESSES; i++) {
        process_table[i].pid = 0;
        process_table[i].state = PROCESS_UNUSED;
        process_table[i].esp = 0;
        process_table[i].entry = 0;
    }

    /*
     * PCB 0 represents the kernel shell that is
     * already running when scheduling starts.
     *
     * Its ESP will be saved during the first
     * timer interrupt.
     */
    process_table[0].pid = 0;
    process_table[0].state = PROCESS_RUNNING;
    process_table[0].esp = 0;
    process_table[0].entry = 0;

    process_count = 1;
    next_pid = 1;
}

int create_process(void (*entry)(void))
{
    int i;
    unsigned int *sp;

    for (i = 1; i < MAX_PROCESSES; i++) {

        if (process_table[i].state == PROCESS_UNUSED) {

            process_table[i].pid = next_pid++;
            process_table[i].state = PROCESS_READY;
            process_table[i].entry = entry;

            /*
             * Start at the top of the 4 KB stack.
             * x86 stacks grow downward.
             */
            sp = (unsigned int *)
                &process_table[i].stack[PROCESS_STACK_SIZE];

            /*
             * Fake an interrupt-return frame.
             *
             * IRETD expects:
             * EIP
             * CS
             * EFLAGS
             */

            *--sp = 0x202;                /* EFLAGS: interrupts enabled */
            *--sp = 0x08;                 /* Kernel code segment */
            *--sp = (unsigned int)entry;  /* Starting EIP */

            /*
             * Fake the PUSHAD register frame.
             *
             * POPAD will restore:
             * EDI, ESI, EBP, ESP(dummy),
             * EBX, EDX, ECX, EAX
             */

            *--sp = 0; /* EAX */
            *--sp = 0; /* ECX */
            *--sp = 0; /* EDX */
            *--sp = 0; /* EBX */
            *--sp = 0; /* Original ESP - ignored by POPAD */
            *--sp = 0; /* EBP */
            *--sp = 0; /* ESI */
            *--sp = 0; /* EDI */

            process_table[i].esp = (unsigned int)sp;

            process_count++;

            return process_table[i].pid;
        }
    }

    return -1;
}

pcb_t *process_get_table(void)
{
    return process_table;
}

int process_get_count(void)
{
    return process_count;
}
