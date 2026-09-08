#ifndef SYSCALL_H
#define SYSCALL_H

#include <stdint.h>

#define SYS_WRITE 1
#define SYS_READ  2
#define SYS_EXIT  3

void syscall_init(void);
int do_syscall(uint32_t num, uint32_t arg1, uint32_t arg2, uint32_t arg3);

#endif
