; TRACELESS OS
; bootloader/boot.asm
; Stage 1 — this is what runs before ANYTHING else

[BITS 16]
[ORG 0x7C00]

start:
    ; Clear screen
    mov ah, 0x00
    mov al, 0x03
    int 0x10

    ; Print message
    mov si, msg
    call print

    ; Hang forever
    cli
    hlt

print:
    lodsb
    or al, al
    jz done
    mov ah, 0x0E
    int 0x10
    jmp print
done:
    ret

msg db "TRACELESS OS - You were never here.", 0

times 510-($-$$) db 0
dw 0xAA55
