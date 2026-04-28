; TRACELESS OS
; kernel/kernel.asm
; 32 bit kernel — we own the machine

[BITS 32]
[ORG 0x8000]

kernel_main:
    ; Print directly to video memory
    ; In 32 bit mode we write straight to 0xB8000
    ; That's the VGA text buffer address
    mov esi, msg
    mov edi, 0xB8000        ; VGA memory
    mov ah, 0x0A            ; color — green on black

.print:
    lodsb
    or al, al
    jz .done
    mov [edi], ax
    add edi, 2
    jmp .print

.done:
    cli
    hlt

msg db "TracelessOS kernel loaded. You were never here.", 0
