#ifndef SYNC_H
#define SYNC_H

#include <stdint.h>

// Mutex structure
typedef struct {
    volatile int locked;
} mutex_t;

// Semaphore structure
typedef struct {
    volatile int count;
} semaphore_t;

// Mutex API
void mutex_init(mutex_t *m);
void mutex_lock(mutex_t *m);
void mutex_unlock(mutex_t *m);

// Semaphore API
void sem_init(semaphore_t *s, int value);
void sem_wait(semaphore_t *s);
void sem_signal(semaphore_t *s);

#endif
