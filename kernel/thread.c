#include "thread.h"
#include "string.h"

// Forward declaration of your process scheduler function
extern int create_process(void (*entry)(void));

static thread_t threads[16];
static uint32_t next_tid = 1;
thread_t *current_thread = 0;

void thread_init(void) {
    memset(threads, 0, sizeof(threads));
}

void thread_yield(void) {
    __asm__ __volatile__("int $0x20");
}

thread_t *thread_create(void (*entry_point)(void)) {
    // Delegate execution to the process manager so the timer switches to it
    create_process(entry_point);
    
    for (int i = 0; i < 16; i++) {
        if (threads[i].state == THREAD_TERMINATED || threads[i].tid == 0) {
            threads[i].tid = next_tid++;
            threads[i].state = THREAD_READY;
            return &threads[i];
        }
    }
    return 0;
}

void thread_exit(void) {
    if (current_thread) {
        current_thread->state = THREAD_TERMINATED;
    }
    thread_yield();
    while (1);
}
