org 0x7c00
bits 16

jmp 0x0000:stage1_main

%include "bpb.inc"

stage1_main:
    cli
    xor ax, ax
    mov ds, ax
    mov es, ax
    mov bp, 0x7c00
    mov sp, bp
    mov ss, ax
    sti

    mov [drive_number], dl

    xor ax, ax
    int 0x12

    jc .memory_error

    mov bx, 0x7c00 + 1024
    mov dl, [drive_number]

    push dx
    mov ah, 0x02

    mov al, 2
    mov ch, 0
    mov dh, 0
    mov cl, 2

    int 0x13

    jc .disk_error

    pop dx
    cmp al, 2

    jne .incomplete_read

    jmp .stage2

.disk_error:
    mov si, msg_error1
    call puts

    jmp .press_key_reboot

.memory_error:
    mov si, msg_error2
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

.stage2:
    mov bx, 0x07c0
    mov es, bx
    mov bx, 0x200
    push es
    push bx

    retf

    jmp .press_key_reboot


msg_loading db 'Loading...', 0x0D, 0x0A, 0
msg_error1 db 'Disk failed', 0x0D, 0x0A, 0
msg_error2 db 'No pp, small ram :(', 0x0D, 0x0A, 0
msg_error3 db 'Incomplete read', 0x0D, 0x0A, 0
msg_error4: db 'Press any key to reboot...', 0x0D, 0x0A, 0

%include "stdio_16.inc"

    ;; padding
times 510 - ($ - $$) db 0
dw 0xAA55                       ; boot signature
