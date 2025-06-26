global idt_reload

idt_reload:
    lidt [rdi]
    ret
