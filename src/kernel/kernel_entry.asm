bits 32

global _start

enter_kernel:
    extern _kmain
    call _kmain

.halt:
    cli
    hlt
    jmp .halt
