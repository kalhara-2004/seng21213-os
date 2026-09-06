#ifndef PROCESS_H
#define PROCESS_H

#include "../include/types.h"

#define MAX_PROCESSES 16
#define PROCESS_STACK_SIZE 4096

typedef enum {
    PROCESS_UNUSED = 0,
    PROCESS_READY,
    PROCESS_RUNNING,
    PROCESS_BLOCKED,
    PROCESS_TERMINATED
} process_state_t;

typedef struct {
    int pid;
    process_state_t state;
    unsigned int esp;
    void (*entry)(void);
    unsigned char stack[PROCESS_STACK_SIZE];
} pcb_t;

void process_init(void);
int create_process(void (*entry)(void));

pcb_t *process_get_table(void);
int process_get_count(void);

#endif
