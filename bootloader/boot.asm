; TRACELESS OS
; bootloader/boot.asm
; Stage 1 — loads stage 2 menu

[BITS 16]
[ORG 0x7C00]

start:
    cli
    xor ax, ax
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov sp, 0x7C00
    mov [boot_drive], dl
    sti

    ; Load stage 2 (menu) from disk
    mov ah, 0x02
    mov al, 4           ; load 4 sectors
    mov ch, 0
    mov cl, 2           ; start at sector 2
    mov dh, 0
    mov dl, [boot_drive]
    mov bx, 0x7E00      ; load right after boot sector
    int 0x13
    jc disk_error

    ; Jump to stage 2
    jmp 0x7E00

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

boot_drive  db 0
msg_error   db "Disk error!", 0

times 510-($-$$) db 0
dw 0xAA55
