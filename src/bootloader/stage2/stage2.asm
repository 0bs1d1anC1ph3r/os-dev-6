org 0x7e00
bits 16

jmp 0x0000:stage2_main

KERNEL_ADDRESS equ 0x8000
NUMBER_OF_SECTORS equ 4

install_gdt:
    cli
    pusha
    lgdt [toc]
    popa
    sti
    ret

gdt_data:
    dd 0                        ; null descriptor
    dd 0

    dw 0FFFFh                   ; limit low
    dw 0                        ; base low
    db 0                        ; base middle
    db 10011010b                ; access
    db 11001111b                ; granularity
    db 0                        ; base high

    dw 0FFFFh                   ; limit low (same as code)
    dw 0                        ; base low
    db 0                        ; base middle
    db 10010010b                ; access
    db 11001111b                ; granularity
    db 0                        ; base high

end_of_gdt:
toc:
    dw end_of_gdt - gdt_data - 1
    dd gdt_data

enable_a20_output_port:
    cli
    pusha

    call wait_input
    mov al, 0xAD
    out 0x64, al                ; disable keyboard
    call wait_input

    mov al, 0xD0
    out 0x64, al                ; tell controller to read output port
    call wait_output

    in al, 0x60
    push eax                    ; get output port data and store it
    call wait_input

    mov al, 0xD1
    out 0x64, al                ; tell controller to write output port
    call wait_input

    pop eax
    or al, 2                    ; set bit 1 (enable a20)
    out 0x60, al                ; write out data back to the output port

    call wait_input
    mov al, 0xAE                ; enable keyboard
    out 0x64, al

    call wait_input

    popa
    sti
    ret

wait_input:
    in al, 0x64
    test al, 2
    jnz wait_input
    ret

wait_output:
    in al, 0x64
    test al, 1
    jz wait_output
    ret

%include "stdio_16.inc"
%include "bpb.inc"

stage2_main:
    cli
    xor ax, ax
    mov ds, ax
    mov es, ax
    mov bp, 0x7c00
    mov sp, bp
    mov ss, ax
    sti

    mov [drive_number], dl

    mov si, msg_test
    call puts

    mov bx, KERNEL_ADDRESS
    mov dh, NUMBER_OF_SECTORS
    mov dl, [drive_number]

    push dx
    mov ah, 0x02

    mov al, dh
    mov ch, 0
    mov dh, 0
    mov cl, 3

    int 0x13


    jc .disk_error

    pop dx
    cmp dh, al

    jne .incomplete_read

    jmp .enter_protected_mode

.disk_error:
    mov si, msg_error1
    call puts

    jmp .press_key_reboot

.incomplete_read:
    mov si, msg_error3
    call puts

    jmp .press_key_reboot

.press_key_reboot:
    mov si, msg_error4
    call puts

    mov ah, 0
    int 0x16
    int 0x19

.enter_protected_mode:
    call install_gdt
    call enable_a20_output_port

    cli

    mov eax, cr0
    or eax, 0x1
    mov cr0, eax

    jmp 08h:init_pm

bits 32

init_pm:
    mov ax, 0x10
    mov ds, ax
    mov ss, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ebp, 0x90000
    mov esp, ebp

    jmp KERNEL_ADDRESS
    jmp $

msg_test db 'AAAAAAAAAAAAAAAA', 0x0D, 0x0A, 0
msg_loading db 'Loading...', 0x0D, 0x0A, 0
msg_error1 db 'Disk failed', 0x0D, 0x0A, 0
msg_error3 db 'Incomplete read', 0x0D, 0x0A, 0
msg_error4: db 'Press any key to reboot...', 0x0D, 0x0A, 0
