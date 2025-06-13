org 0x8000
bits 16

jmp 0x0000:stage2_main

gdt_data:
    dq 0x0000000000000000       ; Null descriptor

    ;; 32-bit code segment
    dq 0x00CF9A000000FFFF

    ;; 32-bit data segment
    dq 0x00CF92000000FFFF

    ;; 64-bit code segment
    dq 0x00AF9A000000FFFF

    ;; 64-bit data segment
    dq 0x00CF92000000FFFF

end_of_gdt:
toc:
    dw end_of_gdt - gdt_data - 1
    dd gdt_data

KERNEL_ADDRESS equ 0x9000
NUMBER_OF_SECTORS equ 4

install_gdt:
    lgdt [toc]
    ret

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

    mov bx, KERNEL_ADDRESS >> 4
    mov es, bx
    xor bx, bx

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
    cli
    pusha
    call install_gdt
    popa
    sti

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
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    mov ebp, 0x90000
    mov esp, ebp

.setup_paging:
    mov eax, cr0
    and eax, 01111111111111111111111111111111b
    mov cr0, eax

    mov edi, 0x1000
    mov cr3, edi
    xor eax, eax
    mov ecx, 4096
    rep stosd
    mov edi, cr3

    mov dword [edi], 0x2003
    add edi, 0x1000
    mov dword [edi], 0x3003
    add edi, 0x1000
    mov dword [edi], 0x4003
    add edi, 0x1000

    mov ebx, 0x00000003
    mov ecx, 512

.set_entry:
    mov dword [edi], ebx
    add ebx, 0x1000
    add edi, 8
    loop .set_entry

    mov eax, cr4
    or eax, 1 << 5
    mov cr4, eax

    mov eax, 0x7
    xor ecx, ecx
    cpuid
    test ecx, (1<<16)
    jnz .5_level_paging

    mov ecx, 0xC0000080
    rdmsr
    or eax, 1 << 8
    wrmsr

    mov eax, cr0
    or eax, 1 << 31
    mov cr0, eax

    jmp 0x18:long_mode_entry

.5_level_paging:
    mov eax, cr4
    or eax, (1<<12)
    mov cr4, eax
    ret

bits 64

long_mode_entry:
    cli
    mov ax, 0x20
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    mov edi, 0xB8000
    mov rax, 0x1F201F201F201F20
    mov ecx, 500
    rep stosq

    hlt

    jmp far [jmp_far_ptr]

jmp_far_ptr:
    dw 0x18               ; segment selector (2 bytes)
    dq 0x9000             ; offset (8 bytes)

msg_loading db 'Loading...', 0x0D, 0x0A, 0
msg_error1 db 'Disk failed', 0x0D, 0x0A, 0
msg_error3 db 'Incomplete read', 0x0D, 0x0A, 0
msg_error4: db 'Press any key to reboot...', 0x0D, 0x0A, 0
