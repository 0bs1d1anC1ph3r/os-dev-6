bits 64

global _start

_start:
    mov rsp, 0x90000
    mov rdi, 0xB8000

    extern _kmain
    call _kmain

.halt:
    cli
    hlt
    jmp .halt
