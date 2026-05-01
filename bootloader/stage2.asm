; TRACELESS OS
; bootloader/bootstage2.asm
; Stage 2 — boot menu

[BITS 16]
[ORG 0x7E00]

start2:
    ; Set video mode
    mov ah, 0x00
    mov al, 0x03
    int 0x10

    ; Hide cursor
    mov ah, 0x01
    mov ch, 0x3F
    int 0x10

    ; Init selected
    mov byte [selected], 0

    call draw_menu

.input_loop:
    mov ah, 0x00
    int 0x16

    cmp ah, 0x1C    ; Enter
    je .boot_selected

    cmp ah, 0x50    ; Down
    je .move_down

    cmp ah, 0x48    ; Up
    je .move_up

    jmp .input_loop

.move_down:
    mov al, [selected]
    cmp al, 3
    je .input_loop
    inc al
    mov [selected], al
    call draw_menu
    jmp .input_loop

.move_up:
    mov al, [selected]
    cmp al, 0
    je .input_loop
    dec al
    mov [selected], al
    call draw_menu
    jmp .input_loop

.boot_selected:
    mov al, [selected]
    cmp al, 3
    je .power_off
    jmp .boot_normal

.power_off:
    mov ax, 0x5307
    mov bx, 0x0001
    mov cx, 0x0003
    int 0x15
    cli
    hlt

.boot_normal:
    cli

    ; Reset disk
    xor ah, ah
    mov dl, 0x80
    int 0x13

    ; Load kernel
    mov ah, 0x02
    mov al, 64
    mov ch, 0
    mov cl, 6           ; kernel starts at sector 6
    mov dh, 0
    mov dl, 0x80
    mov bx, 0x8000
    int 0x13
    jc .disk_error

    ; Load GDT
    lgdt [gdt_descriptor]

    ; Protected mode
    mov eax, cr0
    or eax, 1
    mov cr0, eax
    jmp 0x08:protected_mode

.disk_error:
    mov si, msg_disk_error
    call print16
    cli
    hlt

draw_menu:
    pusha

    ; Clear screen
    mov ah, 0x00
    mov al, 0x03
    int 0x10

    ; Print title area
    mov ah, 0x02
    mov bh, 0x00
    mov dh, 5
    mov dl, 26
    int 0x10
    mov si, border
    call print16

    mov ah, 0x02
    mov dh, 6
    mov dl, 26
    int 0x10
    mov si, title1
    call print16_green

    mov ah, 0x02
    mov dh, 7
    mov dl, 26
    int 0x10
    mov si, title2
    call print16

    mov ah, 0x02
    mov dh, 8
    mov dl, 26
    int 0x10
    mov si, border
    call print16

    ; Print options
    mov byte [row], 10

    mov si, opt0
    mov al, [selected]
    cmp al, 0
    je .sel0
    call print_opt
    jmp .do1
.sel0:
    call print_opt_sel

.do1:
    inc byte [row]
    mov si, opt1
    mov al, [selected]
    cmp al, 1
    je .sel1
    call print_opt
    jmp .do2
.sel1:
    call print_opt_sel

.do2:
    inc byte [row]
    mov si, opt2
    mov al, [selected]
    cmp al, 2
    je .sel2
    call print_opt
    jmp .do3
.sel2:
    call print_opt_sel

.do3:
    inc byte [row]
    mov si, opt3
    mov al, [selected]
    cmp al, 3
    je .sel3
    call print_opt
    jmp .do_footer
.sel3:
    call print_opt_sel

.do_footer:
    mov ah, 0x02
    mov dh, 15
    mov dl, 26
    int 0x10
    mov si, border
    call print16

    mov ah, 0x02
    mov dh, 17
    mov dl, 20
    int 0x10
    mov si, footer
    call print16

    popa
    ret

print_opt:
    pusha
    mov ah, 0x02
    mov bh, 0x00
    mov dh, [row]
    mov dl, 28
    int 0x10
    mov si, prefix_normal
    call print16
    call print16
    popa
    ret

print_opt_sel:
    pusha
    mov ah, 0x02
    mov bh, 0x00
    mov dh, [row]
    mov dl, 28
    int 0x10
    mov si, prefix_sel
    call print16_green
    call print16_green
    popa
    ret

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

print16_green:
    pusha
.loop:
    lodsb
    or al, al
    jz .done
    mov ah, 0x09
    mov bl, 0x0A
    mov bh, 0x00
    mov cx, 1
    int 0x10
    mov ah, 0x0E
    int 0x10
    jmp .loop
.done:
    popa
    ret

; GDT
gdt_start:
gdt_null:   dq 0
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

[BITS 32]
protected_mode:
    mov ax, 0x10
    mov ds, ax
    mov ss, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov esp, 0x90000
    jmp 0x8000

[BITS 16]
selected        db 0
row             db 0

border      db '+-------------------------+', 0
title1      db '|      TRACELESS OS       |', 0
title2      db '|  You were never here.   |', 0
opt0        db 'Boot TracelessOS          ', 0
opt1        db 'Advanced Options          ', 0
opt2        db 'Recovery Mode             ', 0
opt3        db 'Power Off                 ', 0
prefix_normal   db '  ', 0
prefix_sel      db '> ', 0
footer      db 'Arrow keys to navigate. Enter to select.', 0
msg_disk_error  db 'Disk error loading kernel!', 0
