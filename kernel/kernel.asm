; TRACELESS OS
; kernel/kernel.asm
; Stage 2 — we are now the OS

[BITS 16]
[ORG 0x8000]

    mov si, msg
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

msg db "TracelessOS kernel loaded.", 0x0D, 0x0A
    db "You were never here.", 0x0D, 0x0A, 0
