global idt_reload

idt_reload:
    push rbp
    mov rbp, rsp
    mov rax, rdi
    lidt [rax]
    pop rbp
    ret
