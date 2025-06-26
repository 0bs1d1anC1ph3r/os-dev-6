%macro push_all 0
    push rax
    push rbx
    push rcx
    push rdx
    push rsi
    push rdi
    push rbp
    push r8
    push r9
    push r10
    push r11
    push r12
    push r13
    push r14
    push r15
%endmacro

%macro pop_all 0
    pop r15
    pop r14
    pop r13
    pop r12
    pop r11
    pop r10
    pop r9
    pop r8
    pop rbp
    pop rdi
    pop rsi
    pop rdx
    pop rcx
    pop rbx
    pop rax
%endmacro

%macro isr_err_stub 1
  global isr_stub_%+%1
isr_stub_%+%1:
    mov rsi, [rsp]
    mov rdi, %1
    call isr_exception_handler
    add rsp, 8
    iretq
%endmacro

%macro isr_no_err_stub 1
  global isr_stub_%+%1
isr_stub_%+%1:
    xor rsi, rsi
    mov rdi, %1
    call isr_exception_handler
    iretq
%endmacro

%macro irq_stub 1
    global irq_stub_%+%1
irq_stub_%+%1:
    push_all
    mov rdi, %1
    call irq_handler
    pop_all
    iretq
%endmacro

extern isr_exception_handler
extern irq_handler

isr_no_err_stub 0
isr_no_err_stub 1
isr_no_err_stub 2
isr_no_err_stub 3
isr_no_err_stub 4
isr_no_err_stub 5
isr_no_err_stub 6
isr_no_err_stub 7
isr_err_stub    8
isr_no_err_stub 9
isr_err_stub    10
isr_err_stub    11
isr_err_stub    12
isr_err_stub    13
isr_err_stub    14
isr_no_err_stub 15
isr_no_err_stub 16
isr_err_stub    17
isr_no_err_stub 18
isr_no_err_stub 19
isr_no_err_stub 20
isr_no_err_stub 21
isr_no_err_stub 22
isr_no_err_stub 23
isr_no_err_stub 24
isr_no_err_stub 25
isr_no_err_stub 26
isr_no_err_stub 27
isr_no_err_stub 28
isr_no_err_stub 29
isr_err_stub    30
isr_no_err_stub 31
irq_stub 0
irq_stub 1
irq_stub 2
irq_stub 3
irq_stub 4
irq_stub 5
irq_stub 6
irq_stub 7
irq_stub 8
irq_stub 9
irq_stub 10
irq_stub 11
irq_stub 12
irq_stub 13
irq_stub 14
irq_stub 15

section .data

align 8
global isr_stub_table
isr_stub_table:
%assign i 0
%rep    32
    dq isr_stub_%+i
%assign i i+1
%endrep

align 8
global irq_stub_table
irq_stub_table:
%assign i 0
%rep 16
    dq irq_stub_%+i
%assign i i+1
%endrep
