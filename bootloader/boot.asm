; TRACELESS OS
; bootloader/boot.asm
; Stage 1 — loads kernel and hands control

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
    call print

    ; Load kernel from disk
    mov ah, 0x02        ; BIOS read sector
    mov al, 1           ; read 1 sector
    mov ch, 0           ; cylinder 0
    mov cl, 2           ; sector 2 (kernel lives here)
    mov dh, 0           ; head 0
    mov bx, 0x8000      ; load kernel to address 0x8000
    int 0x13            ; BIOS disk interrupt
    jc disk_error       ; jump if error

    ; Jump to kernel
    jmp 0x8000

disk_error:
    mov si, msg_error
    call print
    cli
    hlt

print:
    lodsb
    or al, al
    jz .done
    mov ah, 0x0E
    int 0x10
    jmp print
.done:
    ret

msg_load  db "Loading TracelessOS...", 0x0D, 0x0A, 0
msg_error db "Disk error!", 0x0D, 0x0A, 0

times 510-($-$$) db 0
dw 0xAA55
