#include "syscall.h"
#include "vga.h"
#include "ramfs.h"

void syscall_handler(uint32_t sys_num, uint32_t arg1, uint32_t arg2) {
    switch (sys_num) {
        case SYS_WRITE:
            vga_puts((const char *)arg1);
            break;
        case SYS_READ:
            ramfs_read((const char *)arg1);
            break;
        case SYS_EXIT:
            vga_puts("\n[User Process Terminated]\n");
            break;
        default:
            vga_puts("Unknown System Call!\n");
            break;
    }
}

void syscall_init(void) {
    vga_puts("System Call Handler Initialized (int 0x80).\n");
}
