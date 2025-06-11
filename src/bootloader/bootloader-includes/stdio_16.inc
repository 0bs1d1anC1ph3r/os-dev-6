%ifndef __STDIO_16_BIT_REAL_MODE_INC_INCLUDE__
%define __STDIO_16_BIT_REAL_MODE_INC_INCLUDE__

puts:
    pusha

.print_loop:
    lodsb
    or al, al
    jz .print_loop_done
    mov ah, 0EH
    int 0x10
    jmp .print_loop

.print_loop_done:
    popa
    ret

%endif
