bits 64

global _start

enter_kernel:
    jmp $
    extern _kmain
    call _kmain

.halt:
    cli
    hlt
    jmp .halt
