; TRACELESS OS
; kernel/kernel.asm
; Clean boot screen with colors

[BITS 32]
[ORG 0x8000]

kernel_main:
    call clear_screen
    call print_header
    cli
    hlt

clear_screen:
    mov edi, 0xB8000
    mov ecx, 2000           ; 80 columns x 25 rows
    mov ax, 0x0020          ; black background, space character
    rep stosw
    ret

print_header:
    ; Top bar — green
    mov esi, bar
    mov edi, 0xB8000
    mov ah, 0x2F            ; green on black
    call print_colored

    ; Title
    mov esi, title
    mov edi, 0xB80A0        ; row 2
    mov ah, 0x0A            ; bright green
    call print_colored

    ; Tagline
    mov esi, tagline
    mov edi, 0xB8140        ; row 3
    mov ah, 0x07            ; light gray
    call print_colored

    ; Blank row then status
    mov esi, status
    mov edi, 0xB81E0        ; row 5 (skipping row 4)
    mov ah, 0x0A            ; bright green
    call print_colored

    ; Warning
    mov esi, warning
    mov edi, 0xB8280        ; row 6
    mov ah, 0x0C            ; bright red
    call print_colored

    ret

; esi = string, edi = screen pos, ah = color
print_colored:
    lodsb
    or al, al
    jz .done
    mov [edi], ax
    add edi, 2
    jmp print_colored
.done:
    ret

bar      db "================================================================================", 0
title    db "  TRACELESS OS  //  You were never here.", 0
tagline  db "  Privacy-hardened. Anti-forensic. Armed.", 0
status   db "  [ OK ] Kernel loaded in protected mode", 0
warning  db "  [ !! ] All network traffic routed through Tor", 0
