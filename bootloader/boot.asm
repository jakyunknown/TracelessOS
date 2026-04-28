; TRACELESS OS
; bootloader/boot.asm
; Stage 1 — loads kernel, enters protected mode

[BITS 16]
[ORG 0x7C00]

start:
    ; Set up segments
    xor ax, ax
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov sp, 0x7C00

    ; Print loading message
    mov si, msg_load
    call print16

    ; Load kernel from disk
    mov ah, 0x02
    mov al, 1
    mov ch, 0
    mov cl, 2
    mov dh, 0
    mov bx, 0x8000
    int 0x13
    jc disk_error

    ; Disable interrupts
    cli

    ; Load GDT
    lgdt [gdt_descriptor]

    ; Switch to protected mode
    mov eax, cr0
    or eax, 1
    mov cr0, eax

    ; Jump into 32 bit code
    jmp 0x08:protected_mode

disk_error:
    mov si, msg_error
    call print16
    cli
    hlt

print16:
    lodsb
    or al, al
    jz .done
    mov ah, 0x0E
    int 0x10
    jmp print16
.done:
    ret

; =====================
; GDT — Global Descriptor Table
; Tells CPU how memory is laid out
; =====================
gdt_start:

gdt_null:               ; required null descriptor
    dd 0x00000000
    dd 0x00000000

gdt_code:               ; code segment
    dw 0xFFFF           ; limit low
    dw 0x0000           ; base low
    db 0x00             ; base middle
    db 10011010b        ; access byte
    db 11001111b        ; flags + limit high
    db 0x00             ; base high

gdt_data:               ; data segment
    dw 0xFFFF
    dw 0x0000
    db 0x00
    db 10010010b
    db 11001111b
    db 0x00

gdt_end:

gdt_descriptor:
    dw gdt_end - gdt_start - 1
    dd gdt_start

; =====================
; 32 BIT PROTECTED MODE
; We are now a real OS
; =====================
[BITS 32]

protected_mode:
    ; Set up data segments
    mov ax, 0x10
    mov ds, ax
    mov ss, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov esp, 0x90000

    ; Jump to kernel
    jmp 0x8000

msg_load  db "Loading TracelessOS...", 0x0D, 0x0A, 0
msg_error db "Disk error!", 0x0D, 0x0A, 0

times 510-($-$$) db 0
dw 0xAA55
