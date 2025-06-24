org 0x8000
bits 16

jmp 0x0000:stage2_main

gdt32:
    dq 0
.code32: equ $ - gdt32
    dq 0x00CF9A000000FFFF
.data32: equ $ - gdt32
    dq 0x00CF92000000FFFF
.pointer32:
    dw .pointer32 - gdt32 - 1
    dq gdt32

gdt64:
    dq 0
.code64: equ $ - gdt64
    dq 0x00AF9A000000FFFF
.data64: equ $ - gdt64
    dq 0x00AF92000000FFFF
.pointer64:
    dw .pointer64 - gdt64 - 1
    dq gdt64


KERNEL_BUFFER equ  0x00010000
NUMBER_OF_SECTORS equ 40

install_gdt:
    lgdt [gdt32.pointer32]
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

    mov ax, 0x1000
    mov es, ax
    xor bx, bx

    mov dh, NUMBER_OF_SECTORS
    mov dl, [drive_number]

    push dx
    mov ah, 0x02

    mov al, dh
    mov ch, 0
    mov dh, 0
    mov cl, 4

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
    call install_gdt
    sti

    call enable_a20_output_port

    cli

    mov eax, cr0
    or eax, 0x1
    mov cr0, eax

    jmp 08h:init_pm

msg_loading db 'Loading...', 0x0D, 0x0A, 0
msg_error1 db 'Disk failed', 0x0D, 0x0A, 0
msg_error3 db 'Incomplete read', 0x0D, 0x0A, 0
msg_error4: db 'Press any key to reboot...', 0x0D, 0x0A, 0


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

    mov edi, 0x3000
    mov eax, 0x00000083
    xor ebx, ebx
    mov ecx, 512

.set_entry:
    mov dword [edi], eax
    add eax, 0x200000
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

    lgdt [gdt64.pointer64]

    mov eax, cr0
    or eax, (1 << 31) | 1
    mov cr0, eax

    jmp 0x08:long_mode_entry

.5_level_paging:
    mov eax, cr4
    or eax, (1 << 12)
    mov cr4, eax
    ret

bits 64

long_mode_entry:
    cli
    mov ax, gdt64.data64
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    mov rdi, 0xB8000

    mov rax, KERNEL_BUFFER

    jmp rax
