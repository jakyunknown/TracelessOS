; TRACELESS OS
; bootloader/boot.asm
; Stage 1 — clean rewrite

[BITS 16]
[ORG 0x7C00]

start:
    ; Disable interrupts during setup
    cli

    ; Set up segments
    xor ax, ax
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    mov sp, 0x7C00

    ; Save boot drive
    mov [boot_drive], dl

    ; Re-enable interrupts
    sti

    ; Print loading message
    mov si, msg_load
    call print16

    ; Reset disk controller
    xor ah, ah
    mov dl, [boot_drive]
    int 0x13
    jc disk_error

    ; Load kernel
    mov ah, 0x02        ; read sectors
    mov al, 32          ; read 32 sectors
    mov ch, 0           ; cylinder 0
    mov cl, 2           ; start at sector 2
    mov dh, 0           ; head 0
    mov dl, [boot_drive]; boot drive
    mov bx, 0x8000      ; load to 0x8000
    int 0x13
    jc disk_error

    ; Print success
    mov si, msg_ok
    call print16

    ; Disable interrupts for mode switch
    cli

    ; Load GDT
    lgdt [gdt_descriptor]

    ; Switch to protected mode
    mov eax, cr0
    or eax, 1
    mov cr0, eax

    ; Far jump to flush pipeline
    jmp 0x08:protected_mode

disk_error:
    mov si, msg_error
    call print16
    cli
    hlt

; =====================
; 16 BIT PRINT
; =====================
print16:
    pusha
.loop:
    lodsb
    or al, al
    jz .done
    mov ah, 0x0E
    int 0x10
    jmp .loop
.done:
    popa
    ret

; =====================
; GDT
; =====================
gdt_start:

gdt_null:
    dq 0

gdt_code:
    dw 0xFFFF
    dw 0x0000
    db 0x00
    db 10011010b
    db 11001111b
    db 0x00

gdt_data:
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

; =====================
; DATA
; =====================
boot_drive db 0
msg_load   db "Loading TracelessOS...", 0x0D, 0x0A, 0
msg_ok     db "Kernel found. Jumping to protected mode...", 0x0D, 0x0A, 0
msg_error  db "Disk error! Cannot load kernel.", 0x0D, 0x0A, 0

times 510-($-$$) db 0
dw 0xAA55
