[BITS 32]

[EXTERN timer_handler]
[EXTERN scheduler_switch]

[GLOBAL irq0_handler]

irq0_handler:

    ; Save current process CPU registers.
    ; The CPU has already pushed EIP, CS and EFLAGS.
    pusha

    ; Count the timer tick and send PIC EOI.
    call timer_handler

    ; scheduler_switch(current_esp)
    ; ESP currently points to the saved PUSHAD frame.
    push esp
    call scheduler_switch
    add esp, 4

    ; scheduler_switch returns the next process ESP in EAX.
    mov esp, eax

    ; Restore registers from the selected process.
    popa

    ; Restore EIP, CS and EFLAGS.
    iretd
