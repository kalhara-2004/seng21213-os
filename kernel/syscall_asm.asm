[BITS 32]
global syscall_stub
extern syscall_handler

syscall_stub:
    push ecx            ; arg2
    push ebx            ; arg1
    push eax            ; sys_num
    call syscall_handler
    add esp, 12         ; Clean up pushed arguments
    iretd
