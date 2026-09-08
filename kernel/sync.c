#include "sync.h"
#include "thread.h"

void mutex_init(mutex_t *m) {
    m->locked = 0;
}

void mutex_lock(mutex_t *m) {
    while (__sync_lock_test_and_set(&m->locked, 1)) {
        thread_yield();
    }
}

void mutex_unlock(mutex_t *m) {
    __sync_lock_release(&m->locked);
}


void sem_init(semaphore_t *s, int value) {
    s->count = value;
}

void sem_wait(semaphore_t *s) {
    while (1) {
        while (s->count <= 0) {
            thread_yield();
        }
        if (__sync_bool_compare_and_swap(&s->count, s->count, s->count - 1)) {
            break;
        }
    }
}

void sem_signal(semaphore_t *s) {
    __sync_fetch_and_add(&s->count, 1);
}
