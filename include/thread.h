#ifndef THREAD_H
#define THREAD_H

#include <stdint.h>

typedef enum {
    THREAD_READY,
    THREAD_RUNNING,
    THREAD_BLOCKED,
    THREAD_TERMINATED
} thread_state_t;

typedef struct thread {
    uint32_t tid;
    uint32_t *esp;
    uint32_t stack[1024];
    thread_state_t state;
    struct thread *next;
} thread_t;

void thread_init(void);
thread_t *thread_create(void (*entry_point)(void));
void thread_yield(void);
void thread_exit(void);

#endif
