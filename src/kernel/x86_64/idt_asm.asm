global idt_reload

idt_reload:
    mov rax, rdi
    lidt [rax]
    ret
