#include "syscall.h"
#include "vga.h"
#include "ramfs.h"
#include "process.h"
#include "scheduler.h"
extern int scheduler_get_current(void);

void syscall_handler(uint32_t sys_num, uint32_t arg1, uint32_t arg2) {
    switch (sys_num) {
        case SYS_WRITE:
            vga_puts((const char *)arg1);
            break;
        case SYS_READ:
            ramfs_read((const char *)arg1);
            break;
        case SYS_EXIT: {
            pcb_t *table = process_get_table();
            int current = scheduler_get_current();
            table[current].state = PROCESS_TERMINATED;
            vga_puts("\n[Process Terminated]\n");
            break;
        }
        default:
            vga_puts("Unknown System Call!\n");
            break;
    }
}

void syscall_init(void) {
    vga_puts("System Call Handler Initialized (int 0x80).\n");
}
void sys_write(const char *msg) {
    __asm__ __volatile__ (
        "int $0x80"
        :
        : "a"(SYS_WRITE), "b"(msg)
    );
}

void sys_read(const char *path) {
    __asm__ __volatile__ (
        "int $0x80"
        :
        : "a"(SYS_READ), "b"(path)
    );
}

void sys_exit(void) {
    __asm__ __volatile__ (
        "int $0x80"
        :
        : "a"(SYS_EXIT)
    );
}
